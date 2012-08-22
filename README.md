# zmqcat

一个将标准输入以ZMQ广播方式广播的工具。这样可以将日志文件通过ZMQ广播出去。


例如将syslog通过5678端口广播，可以通过以下命令

```sh
$ tail -f /var/log/syslog | zmqcat "tcp://*:5678"
```

然后再写一个接收程序处理这些信息。下面示例的python程序简单地将收到的信息输出。

```python
import zmq

context = zmq.Context()
socket = context.socket(zmq.SUB)

socket.connect("tcp://127.0.0.1:5678")
socket.setsockopt(zmq.SUBSCRIBE, "")

while True:
    s = socket.recv()
    print s,
```

有的时候，日志文件非常大，我们希望能够直接处理这些日志，而不需要写入硬盘后再从硬盘中读出处理。

例如xdebug的输出只能定义到文件，而且输出量特别大。我们可以在xdebug中配置，将输出文件指定到一个FIFO设备(例如xdebug.fifo)，并且通过zmqcat广播。

```sh
$ mkfifo xdebug.fifo
$ tail -f xdebug.fifo | zmqcat
```

注意：当没有进程读取这个FIFO文件时，xdebug的日志写入操作将被block。

