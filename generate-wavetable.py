import math
import sys

length = int(sys.argv[1])

values = []
for n in xrange(length):
	v = math.cos(2.0*math.pi*float(n)/float(length))*0.8
	v += math.cos(4.0*math.pi*float(n)/float(length))*0.4
	#v += math.cos(6.0*math.pi*float(n)/float(length))*0.2
	#v += math.cos(8.0*math.pi*float(n)/float(length))*0.1

	values.append(v)

minv = min(values)
maxv = max(values)

values = [ (v - minv)*254.0/(maxv - minv) - 127.0 for v in values ]

assert min(values) == -127
assert max(values) == 127

sys.stdout.write('#include "wavetable.h"\n\n');

sys.stdout.write("const size_t wavetable_len = %d;\n\n" % length);

sys.stdout.write("const int8_t wavetable[] = {")

for n, v in enumerate(values):
	if n % 8 == 0:
		sys.stdout.write("\n\t")

	sys.stdout.write("%4d" % v)

	if n < length - 1:
		sys.stdout.write(", ")

assert n == length - 1

sys.stdout.write("\n};\n")
