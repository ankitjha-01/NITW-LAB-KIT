from mininet.net import Mininet
from mininet.node import OVSSwitch

net = Mininet(controller=None, switch=OVSSwitch)

h1 = net.addHost('h1', ip='10.0.0.1/24')
h2 = net.addHost('h2', ip='10.0.0.2/24')
h3 = net.addHost('h3', ip='10.0.0.3/24')
h4 = net.addHost('h4', ip='10.0.0.4/24')

s1 = net.addSwitch('s1', failMode='standalone')

net.addLink(h1,s1)
net.addLink(h2,s1)
net.addLink(h3,s1)
net.addLink(h4,s1)

net.start()

# runn a ping command from the host h1 to h2 and store the output in the result variable
result = h1.cmd('ping -c 3 10.0.0.2')

print(result)

net.stop()
