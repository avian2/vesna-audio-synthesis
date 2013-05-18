import math
import sys

length = int(sys.argv[1])

sys.stdout.write('#include "wavetable.h"\n\n');

sys.stdout.write("const size_t wavetable_len = %d;\n\n" % length);

sys.stdout.write("const int8_t wavetable[] = {")

for n in xrange(length):
	if n % 8 == 0:
		sys.stdout.write("\n\t")

	v = math.cos(2.0*math.pi*float(n)/float(length))*0.8/1.5
	v += math.cos(4.0*math.pi*float(n)/float(length))*0.4/1.5
	v += math.cos(6.0*math.pi*float(n)/float(length))*0.2/1.5
	v += math.cos(8.0*math.pi*float(n)/float(length))*0.1/1.5

	c = v * 127.0

	sys.stdout.write("%4d" % c)

	if n < length - 1:
		sys.stdout.write(", ")

sys.stdout.write("\n};\n")
