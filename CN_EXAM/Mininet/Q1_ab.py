# Q1_a : sudo mn --topo single,2
# mininet> pingall
# mininet> h1 ping h2
# mininet> h1 iperf h2

# Q1_b :
from mininet.net import Mininet
from mininet.cli import CLI
from mininet.node import OVSSwitch

net = Mininet(switch=OVSSwitch, controller=None)

h1 = net.addHost('h1')
h2 = net.addHost('h2')
h3 = net.addHost('h3')

s1 = net.addSwitch("s1", failMode='standalone')
s2 = net.addSwitch("s2", failMode='standalone')

net.addLink(h1, s1)
net.addLink(h2, s1)
net.addLink(h3, s1)

net.addLink(h3, s2)

net.addLink(s1, s2)

net.start()

net.pingAll()

# Q_3 : Simulate the failure of h3 and test the ping between each host in the network again
print("Simulating failure of h3")
# (close the connecion h3 --> s1)
h3.cmd('ifconfig h3-eth0 down')
# (close the connecion h3 --> s2)
h3.cmd('ifconfig h3-eth1 down')
net.pingAll()

# CLI(net)

net.stop()
