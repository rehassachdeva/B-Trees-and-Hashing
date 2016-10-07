import sys, csv, random, os

# r% duplicates
r = int(sys.argv[1])

filename = sys.argv[2]

# Range of entries in tuples
MIN = 1
MAX = 100

# len of tuple
l = random.randrange(1,6)

while True:

	with open(filename, 'a') as csvfile:

		writer = csv.writer(csvfile, delimiter=',')	

		# To keep track of tuples generated
		track = {}

		records_100 = []

		while True:

			# Generate new tuple
			cur = ()
			for j in range(l):
				new = random.randint(MIN, MAX)
				new = str(new)
				cur += (new, )

			if cur in track.keys():
				continue
			else:
				track[cur] = True
				records_100.append(cur)

			if len(records_100) == 100:
				break

		indices_repeated = random.sample(range(100), r)

		for index in indices_repeated:
			records_100.append(records_100[index])

		for record in records_100:
			writer.writerow(list(record))

	csvfile.close()

	# Check size of file so far
	cur_size = os.stat(filename).st_size

	if cur_size > 1e9:
		break