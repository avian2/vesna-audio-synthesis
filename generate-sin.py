import math
import sys

length = int(sys.argv[1])

sys.stdout.write('#include "sin.h"\n\n');

sys.stdout.write("const size_t sin_data_len = %d;\n\n" % length);

sys.stdout.write("const int8_t sin_data[] = {")

for n in xrange(length):
	if n % 8 == 0:
		sys.stdout.write("\n\t")

	v = math.cos(2.0*math.pi*float(n)/float(length))

	c = v * 127.0

	sys.stdout.write("%4d" % c)

	if n < length - 1:
		sys.stdout.write(", ")

sys.stdout.write("\n};\n")
