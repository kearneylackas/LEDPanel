import matplotlib.pyplot as plt
import numpy

w = 7195
h = 3841

im_np = numpy.random.rand(h, w)

fig = plt.figure(frameon=False)
fig.set_size_inches(w,h)
ax = plt.Axes(fig, [0., 0., 1., 1.])
ax.set_axis_off()
fig.add_axes(ax)
ax.imshow(im_np, aspect='normal')
fig.savefig('figure.png', dpi=1)