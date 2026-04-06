from mininet.net import Mininet
from mininet.node import OVSSwitch

net = Mininet(controller=None, switch=OVSSwitch)

h1 = net.addHost('h1')
h2 = net.addHost('h2')
h3 = net.addHost('h3')
h4 = net.addHost('h4')

s1 = net.addSwitch('s1', failMode='standalone')
s2 = net.addSwitch('s2', failMode='standalone')

net.addLink(h1,s1)
net.addLink(h2,s1)

net.addLink(h3,s2)
net.addLink(h4,s2)

net.addLink(s1,s2)

net.start()

net.pingAll()
net.iperf((h1,h4))

net.stop()
