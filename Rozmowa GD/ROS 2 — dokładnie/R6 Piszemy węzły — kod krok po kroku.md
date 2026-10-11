---
typ: pojęcie
status: nowe
przedmioty: []
wymaga: ["[[R2 Węzły, executory, callbacki i czas]]", "[[R5 Workspace, pakiety, colcon i launch]]"]
powiązane: ["[[R0 ROS 2 — mapa działu]]", "[[18 Python i C++ — minimum na rozmowę]]", "[[R7 ROS 2 w labie zbierania danych]]"]
tags: [pojęcie, rozmowa-gd, ros2, kod]
utworzono: 2026-10-10
źródła: ["https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Writing-A-Simple-Py-Publisher-And-Subscriber.html", "https://docs.ros.org/en/jazzy/Tutorials/Intermediate/Writing-an-Action-Server-Client/Py.html"]
---
# R6 Piszemy węzły — kod krok po kroku

> [!skrot] W 30 sekund
> - Węzeł w Pythonie = klasa dziedzicząca po `rclpy.node.Node`. W `__init__` tworzysz: `create_publisher`, `create_subscription`, `create_timer`, `create_service`, `create_client`, `ActionClient`. Funkcja `main()` robi `init → spin → shutdown`.
> - Pięć wzorców poniżej pokrywa ~90% tego, co napiszesz w labie: **publisher z timerem**, **subscriber**, **serwer serwisu**, **klient serwisu (async)**, **klient akcji**. Plus to samo w **C++**.
> - Na rozmowie mogą dać kartkę: „napisz subskrybenta `/joint_states`, który loguje pozycję chwytaka”. Ten plik to trening.

> [!intuicja] Po ludzku
> Pisanie węzła to wypełnianie formularza: „chcę nadawać na kanale X” (publisher), „chcę słuchać kanału Y, a gdy coś przyjdzie — wywołaj tę funkcję” (subscriber), „co 0,1 s wywołaj tamtą” (timer). Potem oddajesz formularz executorowi (`spin`) i on pilnuje, żeby wszystko działo się na czas.

## 1. Publisher z timerem — „heartbeat stanowiska”
```python
import rclpy
from rclpy.node import Node
from std_msgs.msg import String

class Heartbeat(Node):
    def __init__(self):
        super().__init__("heartbeat")                        # nazwa węzła w grafie ROS
        self.declare_parameter("station", "st1")             # parametr z domyślną wartością
        self.pub = self.create_publisher(String, "heartbeat", 10)   # 10 = KEEP_LAST depth, RELIABLE
        self.timer = self.create_timer(1.0, self.tick)       # co 1 s
        self.count = 0

    def tick(self):
        msg = String()
        msg.data = f"{self.get_parameter('station').value} alive #{self.count}"
        self.pub.publish(msg)
        self.get_logger().info(msg.data)                     # logi: ros2 run ... / rqt_console
        self.count += 1

def main():
    rclpy.init()
    node = Heartbeat()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.try_shutdown()

if __name__ == "__main__":
    main()
```
Linijka po linijce: `super().__init__("heartbeat")` rejestruje węzeł; `create_publisher(typ, nazwa, qos)` — nazwa **bez** `/` jest **względna** (dostanie namespace z launcha); `create_timer(okres, callback)`; `spin` oddaje sterowanie executorowi.

## 2. Subscriber — monitor chwytaka z `/joint_states`
```python
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState

class GripperMonitor(Node):
    def __init__(self):
        super().__init__("gripper_monitor")
        self.sub = self.create_subscription(JointState, "/joint_states", self.on_js, 10)
        self.last_stamp = None

    def on_js(self, msg: JointState):
        if "gripper" not in msg.name:                    # nazwy stawów są w tablicy name[]
            return
        i = msg.name.index("gripper")
        pos = msg.position[i]
        t = msg.header.stamp.sec + msg.header.stamp.nanosec * 1e-9
        if self.last_stamp is not None and t - self.last_stamp > 0.1:   # luka > 100 ms
            self.get_logger().warn(f"gap in joint_states: {t - self.last_stamp:.3f}s")
        self.last_stamp = t
        self.get_logger().info(f"gripper = {pos:.3f}", throttle_duration_sec=1.0)  # max raz na s

def main():
    rclpy.init(); rclpy.spin(GripperMonitor()); rclpy.shutdown()
```
> [!wazne] Nie zakładaj kolejności stawów
> `msg.name` może mieć inną kolejność niż w URDF, a niektóre sterowniki publikują **kilka** wiadomości JointState z różnymi podzbiorami stawów. Zawsze szukaj po nazwie. To częsty błąd przy konwersji bagów do datasetu.

## 3. Serwer serwisu — „start/stop nagrywania”
```python
from std_srvs.srv import SetBool

class RecorderControl(Node):
    def __init__(self):
        super().__init__("recorder_control")
        self.recording = False
        self.srv = self.create_service(SetBool, "set_recording", self.on_set)

    def on_set(self, request, response):
        self.recording = request.data
        response.success = True
        response.message = "recording" if self.recording else "stopped"
        self.get_logger().info(response.message)
        return response                                   # ZAWSZE zwróć response
```
`ros2 service call /set_recording std_srvs/srv/SetBool "{data: true}"`

## 4. Klient serwisu — asynchronicznie, z timeoutem
```python
from std_srvs.srv import Trigger

class Calibrator(Node):
    def __init__(self):
        super().__init__("calibrator")
        self.cli = self.create_client(Trigger, "/gripper/calibrate")

    def calibrate(self) -> bool:
        if not self.cli.wait_for_service(timeout_sec=2.0):
            self.get_logger().error("service not available")
            return False
        future = self.cli.call_async(Trigger.Request())
        rclpy.spin_until_future_complete(self, future, timeout_sec=5.0)   # OK poza callbackiem!
        if not future.done():
            self.get_logger().error("timeout"); return False
        res = future.result()
        self.get_logger().info(f"{res.success}: {res.message}")
        return res.success

def main():
    rclpy.init(); node = Calibrator(); node.calibrate(); node.destroy_node(); rclpy.shutdown()
```
> [!blad] `spin_until_future_complete` **w callbacku** to błąd — executor już kręci, grozi zawieszenie. W callbacku: `future.add_done_callback(...)`. Patrz [[R2 Węzły, executory, callbacki i czas]].

## 5. Klient akcji — wyślij ramię do pozycji „home”
```python
import rclpy
from rclpy.node import Node
from rclpy.action import ActionClient
from control_msgs.action import FollowJointTrajectory
from trajectory_msgs.msg import JointTrajectoryPoint
from builtin_interfaces.msg import Duration

JOINTS = ["shoulder_pan_joint", "shoulder_lift_joint", "elbow_joint",
          "wrist_1_joint", "wrist_2_joint", "wrist_3_joint"]
HOME = [0.0, -1.57, 1.57, -1.57, -1.57, 0.0]

class GoHome(Node):
    def __init__(self):
        super().__init__("go_home")
        self.ac = ActionClient(self, FollowJointTrajectory,
                               "/scaled_joint_trajectory_controller/follow_joint_trajectory")

    def send(self):
        self.ac.wait_for_server()
        goal = FollowJointTrajectory.Goal()
        goal.trajectory.joint_names = JOINTS
        pt = JointTrajectoryPoint(positions=HOME, time_from_start=Duration(sec=6))   # wolno!
        goal.trajectory.points = [pt]
        fut = self.ac.send_goal_async(goal, feedback_callback=self.on_fb)
        fut.add_done_callback(self.on_accepted)

    def on_fb(self, fb):
        err = fb.feedback.error.positions
        self.get_logger().info(f"max err {max(abs(e) for e in err):.3f} rad", throttle_duration_sec=0.5)

    def on_accepted(self, fut):
        handle = fut.result()
        if not handle.accepted:
            self.get_logger().error("goal rejected"); rclpy.shutdown(); return
        handle.get_result_async().add_done_callback(self.on_result)

    def on_result(self, fut):
        r = fut.result().result
        self.get_logger().info(f"done: error_code={r.error_code} {r.error_string}")
        rclpy.shutdown()

def main():
    rclpy.init(); n = GoHome(); n.send(); rclpy.spin(n)
```
Łańcuch callbacków: `send_goal_async` → *accepted?* → `get_result_async` → *result*. Anulowanie: `handle.cancel_goal_async()`.
(Dla UR nazwa kontrolera to zwykle `scaled_joint_trajectory_controller`; sprawdź `ros2 control list_controllers`.)

## 6. To samo w C++ — publisher + subscriber
```cpp
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <std_msgs/msg/float64.hpp>

class GripperRepub : public rclcpp::Node {
public:
  GripperRepub() : Node("gripper_repub") {
    pub_ = create_publisher<std_msgs::msg::Float64>("gripper_pos", 10);
    sub_ = create_subscription<sensor_msgs::msg::JointState>(
      "/joint_states", 10,
      [this](const sensor_msgs::msg::JointState::SharedPtr msg) {     // lambda jako callback
        for (size_t i = 0; i < msg->name.size(); ++i) {
          if (msg->name[i] == "gripper") {
            std_msgs::msg::Float64 out; out.data = msg->position[i];
            pub_->publish(out);
          }
        }
      });
  }
private:
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr pub_;
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr sub_;
};

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<GripperRepub>());
  rclcpp::shutdown();
  return 0;
}
```
| Python | C++ |
|---|---|
| `self.create_publisher(T, "x", 10)` | `create_publisher<T>("x", 10)` |
| callback = metoda | callback = lambda / `std::bind` |
| `msg.position[i]` | `msg->position[i]` (SharedPtr) |
| `self.get_logger().info(...)` | `RCLCPP_INFO(get_logger(), "...%f", x)` |
| łatwo, wolniej | szybciej, intra-process zero-copy, real-time |

## 7. Wzorzec „rekorder z synchronizacją” (bardzo labowy)
```python
from message_filters import Subscriber, ApproximateTimeSynchronizer
from rclpy.qos import qos_profile_sensor_data
from sensor_msgs.msg import Image, JointState

class SyncedSampler(Node):
    def __init__(self):
        super().__init__("synced_sampler")
        img = Subscriber(self, Image, "/wrist_cam/color/image_raw", qos_profile=qos_profile_sensor_data)
        js = Subscriber(self, JointState, "/joint_states")
        self.sync = ApproximateTimeSynchronizer([img, js], queue_size=30, slop=0.02)   # ±20 ms
        self.sync.registerCallback(self.on_pair)

    def on_pair(self, img: Image, js: JointState):
        dt = abs((img.header.stamp.nanosec - js.header.stamp.nanosec) * 1e-9 +
                 (img.header.stamp.sec - js.header.stamp.sec))
        self.get_logger().info(f"pair dt={dt*1000:.1f} ms", throttle_duration_sec=1.0)
```
`ApproximateTimeSynchronizer` łączy wiadomości z **najbliższymi znacznikami czasu** w granicy `slop`. Działa tylko, jeśli `header.stamp` jest sensowny. W praktyce w datasetach częściej nagrywa się wszystko do bagu i **resampluje offline** — patrz [[R7 ROS 2 w labie zbierania danych]].

## Ćwiczenie (zrób w Dockerze, 30 min)
- [ ] Pakiet `lab_tools` (ament_python), węzły 1 i 2. `ros2 run` obu, `ros2 topic hz /heartbeat`.
- [ ] Publikuj sztuczne `/joint_states` z `ros2 topic pub -r 50 /joint_states sensor_msgs/msg/JointState "{name: [gripper], position: [0.5]}"` i odpal monitor.
- [ ] Zatrzymaj publisher na 2 s → zobacz ostrzeżenie o luce.

## Pytania rekrutacyjne
> [!zadanie] Napisz (słownie) węzeł, który subskrybuje `/joint_states` i ostrzega, gdy wiadomości przestaną przychodzić.
> > [!odpowiedz]- Odpowiedź
> > Klasa dziedzicząca po Node; subskrypcja JointState, w callbacku zapisuję czas ostatniej wiadomości (`self.get_clock().now()`); timer co 0,1 s sprawdza, czy od ostatniej minęło więcej niż np. 0,2 s — jeśli tak, `get_logger().warn` (albo publikuję zdarzenie). Alternatywa: QoS **deadline** z callbackiem zdarzenia.

> [!zadanie] Jak wyślesz ramię do pozycji w ROS 2 z kodu?
> > [!odpowiedz]- Odpowiedź
> > ActionClient dla `control_msgs/action/FollowJointTrajectory` do kontrolera ros2_control (np. `scaled_joint_trajectory_controller`): goal z `joint_names` i punktami `positions` + `time_from_start`, `send_goal_async`, sprawdzam accepted, potem `get_result_async` i `error_code`. Na wyższym poziomie: MoveIt (planowanie z kolizjami).

> [!zadanie] Dlaczego szukasz stawu po nazwie, a nie po indeksie w JointState?
> > [!odpowiedz]- Odpowiedź
> > Kolejność w `name[]` nie jest gwarantowana i może się różnić między sterownikami; różne źródła mogą publikować różne podzbiory stawów na tym samym topiku. Indeks na sztywno może przypisać pozycję łokcia do chwytaka — po cichu psując dataset.

> [!zadanie] Czym różni się callback w Pythonie i w C++ w ROS 2?
> > [!odpowiedz]- Odpowiedź
> > W Pythonie to zwykle metoda klasy; w C++ lambda lub `std::bind`, wiadomość przychodzi jako `SharedPtr` (dostęp przez `->`). C++ daje lepszą wydajność, intra-process zero-copy i przewidywalność czasową.

## Fiszki #fiszki/rozmowa-gd/ros-dokladnie
Szkielet main() w rclpy?::rclpy.init() → node = MyNode() → rclpy.spin(node) → node.destroy_node() → rclpy.shutdown().
Co oznacza 10 w create_publisher(T, "x", 10)?::QoS: KEEP_LAST z depth 10 (domyślnie RELIABLE).
Nazwa topiku bez / na początku?::Względna — dostaje namespace węzła.
Jak logować rzadziej w rclpy?::get_logger().info(..., throttle_duration_sec=1.0)
Łańcuch klienta akcji w rclpy?::send_goal_async → handle.accepted → get_result_async → result.
Jak anulować cel akcji?::goal_handle.cancel_goal_async().
Narzędzie do łączenia wiadomości po czasie?::message_filters.ApproximateTimeSynchronizer (queue_size, slop).
Logowanie w C++?::RCLCPP_INFO(get_logger(), "...").
Publikowanie z CLI z częstotliwością?::ros2 topic pub -r 50 /topik typ "{...}"

## Moje notatki
> [!ja] Tu piszesz ty — Albert tej sekcji nie rusza.
