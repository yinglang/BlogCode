import zmq
import time

context = zmq.Context()
socket = context.socket(zmq.SUB)
socket.bind("tcp://127.0.0.1:6000")
# set subscribe topic
# socket.setsockopt(zmq.SUBSCRIBE, "m_topic_data")
socket.subscribe("m_topic_data")
for i in range(4):
    # recv topic+" " + string
    msg = socket.recv()        # recv one of [socket.send_string(topic, zmq.SNDMORE);socket.send_string(data)]
    print(msg)
    # recv as a list
    msg = socket.recv_multipart() # recv all data [socket.send_string(topic, zmq.SNDMORE);socket.send_string(data)]
    print(msg)
    time.sleep(1)