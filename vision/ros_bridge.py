"""PC 코드 → ROS 2 토픽 다리.

ROS 2가 설치된 PC(민혁 PC)에서는 실제 토픽으로 발행하고,
ROS 2가 없는 PC(동민 맥)에서는 같은 내용을 터미널에 출력만 한다.
그래서 같은 코드를 양쪽에서 그대로 실행할 수 있다.

메시지 형식은 docs/PRD.md 5절: std_msgs/String 안에 JSON 문자열.
"""
import json
import time

try:
    import rclpy
    from rclpy.node import Node
    from std_msgs.msg import String
    HAS_ROS = True
except ImportError:  # ROS 2가 없는 PC
    HAS_ROS = False


class TopicPublisher:
    """dict를 JSON으로 바꿔서 토픽에 보낸다. ROS가 없으면 print."""

    def __init__(self, topic, node_name="xiao_vision"):
        self.topic = topic
        self.node = None
        self.pub = None
        if HAS_ROS:
            if not rclpy.ok():
                rclpy.init()
            self.node = Node(node_name)
            self.pub = self.node.create_publisher(String, topic, 10)
            print(f"[ROS 2] {topic} 발행 시작")
        else:
            print(f"[ROS 2 없음] {topic} 내용을 터미널에 출력만 합니다")

    def publish(self, data):
        data = {**data, "stamp": round(time.time(), 3)}  # 보낸 시각 (지연 시간 측정용)
        text = json.dumps(data, ensure_ascii=False)
        if self.pub is not None:
            self.pub.publish(String(data=text))
        else:
            print(f"{self.topic} {text}")

    def close(self):
        if self.node is not None:
            self.node.destroy_node()
            if rclpy.ok():
                rclpy.shutdown()
