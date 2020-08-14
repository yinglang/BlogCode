import zmq
import sys

context = zmq.Context()
socket = context.socket(zmq.SUB)
# socket.connect("tcp://39.106.11.90:6002")
socket.bind("tcp://127.0.0.1:5555")
socket.subscribe("m_topic_data")

for i in range(100):
    # data = input("input your data:")
    #if data == 'q':
    #    sys.exit()

    # socket.send_string(data)
    socket.send("m_topic_data", zmq.SNDMORE)
    response = socket.send("hello world")
    # response = socket.recv();
    # print(response)
