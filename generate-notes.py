import sys
import numpy
import struct

fs = 43.9796e3

def note_to_f(note):
	return (2.0**((float(note)-69.0)/12.0)) * 440.0

def timestamp_to_s(ticks, ticks_per_qn, us_per_qn):
	return 1.0e-6*float(ticks)/float(ticks_per_qn)*float(us_per_qn)

def main():
	fin = open(sys.argv[1])

	track = 2

	sys.stdout.write('#include "event.h"\n\n')

	sys.stdout.write('const struct event events[] = {\n')

	start = True

	count = 0

	for line in fin:

		fields = [ f.strip() for f in line.split(",") ]

		cmd = fields[2]

		if cmd == 'Header':
			ticks_per_qn = int(fields[5])
			continue

		if cmd == 'Tempo':
			us_per_qn = int(fields[3])
			continue

		if int(fields[0]) != track:
			continue

		time = timestamp_to_s(fields[1], ticks_per_qn, us_per_qn)

		if cmd == 'Note_on_c':
			f = note_to_f(fields[4])
			if start:
				start = False
			else:
				sys.stdout.write(",\n");
			sys.stdout.write('\t{ .time = %d, .type = 1, .freq = %f }' % (time*1e3, f));
			count += 1
		elif cmd == 'Note_off_c':
			f = note_to_f(fields[4])
			sys.stdout.write(',\n\t{ .time = %d, .type = 0, .freq = %f }' % (time*1e3, f));
			count += 1

	sys.stdout.write("\n};\n\n");

	sys.stdout.write("const size_t events_num = %d;\n" % (count,))

main()