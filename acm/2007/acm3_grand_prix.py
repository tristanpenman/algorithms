import sys
import math

def normalise(point):
	if (point[0] == 0 and point[1] == 0):
		return point
	r = math.sqrt(point[0] * point[0] + point[1] * point[1])
	return [point[0] / r, point[1] / r]

case = 0
while True:

	case = case + 1

	# Read header for this case
	line = sys.stdin.readline()
	if line is None or line == '':
		break
	
	# Parse header for this case
	header = line.rstrip().split(' ')
	num_segments = int(header[0])
	angle = float(header[1])
	if num_segments == 0 and angle == 0:
		# End of input
		break

	if case > 1:
		print ""

	# Read points for this case
	points = [[0, 0]]
	for n in range(0, num_segments):
		line = sys.stdin.readline()
		point = line.rstrip().split(' ')
		points.append([float(point[0]), float(point[1])])

	# If the course has no elevation, it is not possible to go down-hill
	if angle == 0:
		print "Case " + str(case) + ": Acceptable as proposed"
		continue

	# Otherwise, check each segment
	for n in range(0, num_segments):
		d = [points[n + 1][0] - points[n][0], points[n + 1][1] - points[n][1]] 
		n = math.atan2(d[1], d[0]) 
		d = math.degrees(n)
		if d < 0:
			# Calculate minimum angle of adjustment
			if d < -135:
				d = 270 + d
				d = round(d, 2)
				print "Case " + str(case) + ": Need to rotate point clockwise by " + str(d) + " degrees" 
			else:
				d = round(d, 2)
				print "Case " + str(case) + ": Need to rotate point counter-clockwise by " + str(-d) + " degrees"


	print "Case " + str(case) + ": Unacceptable"
	
