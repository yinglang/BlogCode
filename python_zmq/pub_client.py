import zmq
import time

context = zmq.Context()
socket = context.socket(zmq.PUB)
socket.connect("tcp://127.0.0.1:6000")

topic = "m_topic_data"
for i in range(10):
    data = "hello world 1"
    string = ("{} {}".format(topic, data))
    print("send", string)
    socket.send_string(topic, zmq.SNDMORE)
    socket.send_string(data)
    
    data = "hello world 2"
    string = ("{} {}".format(topic, data))
    print("send", string)
    socket.send_string(string)
    time.sleep(1)