#
# Consanguine Calculations
#
# Sample input:
#  O+ O- ?
#  O+ ? O-
#  AB- AB+ ?
#  AB+ ? O+
#  ^D
# 
# Sample output:
#  O+ O- ?: O+ O- ['O+', 'O-'] 
#  O+ ? O-: O+ ['A+', 'A-', 'B+', 'B-', 'O+', 'O-'] O- 
#  AB- AB+ ?: AB- AB+ ['A+', 'A-', 'AB+', 'AB-', 'B+', 'B-'] 
#  AB+ ? O+: AB+ IMPOSSIBLE O+
#
# Time taken: about 2.5 hours
#
# Issues:
#  Should have planned solution before coding, either in pseudo code, or as unimplemented
#  functions. Most of algorithms started out as heavily nested loops, though most of them
#  were refactored before the program was complete. Returning sorted lists seems to be a
#  good convention to follow, unless it is known to cause performance issues.
#

import sys

def is_A(bt):
	""" Returns true if blood type is A """
	return bt.find('A') != -1 and bt.find('AB') == -1

def is_B(bt):
	""" Returns true if blood type is B """
	return bt.find('B') != -1 and bt.find('AB') == -1

def is_AB(bt):
	""" Returns true if blood type is AB """
	return bt.find('AB') != -1

def is_O(bt):
	""" Returns true if blood type is O """
	return bt.find('O') != -1

def is_positive(bt):
	""" Returns true if blood type is Rh positive """
	return bt.find('+') != -1

def get_genes_for_bloodtype(bt):
	""" Return an array containing the allele genes for the given blood type """
	comb = []
	if is_A(bt):
		comb.append('AA')
		comb.append('AO')
	elif is_AB(bt):
		comb.append('AB')
	elif is_B(bt):
		comb.append('BB')
		comb.append('BO')
	elif is_O(bt):
		comb.append('OO')
	end = len(comb)
	if is_positive(bt):
		for n in range(0, end):
			comb.append(comb[n] + '+-')
			comb[n] = comb[n] + '++'
	else:
		for n in range(0, end):
			comb[n] = comb[n] + '--'
	return comb
			
def find_child_genes(p1, p2):
	""" Returns a sorted list of possible child genes given two lists of parent genes """
	genes = []

	# For each pair of genes
	for p1_gene in p1:
		for p2_gene in p2:

			# For each pair of base alleles in each gene pair
			bases = []
			for p1_allele in range(0, 2):
				for p2_allele in range(0, 2):
					bases.append(p1_gene[p1_allele] + p2_gene[p2_allele])

			# And finally, for each pair of rh alleles
			RHs = []
			for p1_rh in range(2, 4):
				for p2_rh in range(2, 4):
					RHs.append(p1_gene[p1_rh] + p2_gene[p2_rh])

			# Combine base allele pairs with RH gene pairs
			for base in bases:
				for rh in RHs:
					genes.append(base + rh)

	# Remove duplicates from list
	return sorted(list(set(genes)))

def is_valid_candidate_parent(child_genes, parent_gene, candidate_gene):
	""" Returns true if the candidate gene could contribute to one of the child genes """
	potential_child_genes = find_child_genes([parent_gene], [candidate_gene])
	for potential_child_gene in potential_child_genes:
		for child_gene in child_genes:
			if potential_child_gene == child_gene:
				return True

	return False

def find_parent_genes(parent_genes, child_genes):
	""" Returns a sorted list of possible parent genes given a child and the other parent """

	# Generate the complete list of possible genes
	candidate_genes = []
	for base_1 in ['A', 'B', 'O']:
		for base_2 in ['A', 'B', 'O']:
			for rh_1 in ['+', '-']:
				for rh_2 in ['+', '-']:
					candidate_genes.append(base_1 + base_2 + rh_1 + rh_2)

	# Determine the validity of each gene
	confirmed_genes = []
	for candidate_gene in candidate_genes:
		for parent_gene in parent_genes:
			if is_valid_candidate_parent(child_genes, parent_gene, candidate_gene):
				confirmed_genes.append(candidate_gene)

	return sorted(list(set(confirmed_genes)))

def get_bloodtypes_from_genes(genes):
	""" Returns a sorted list of bloodtypes that result from an array of genes """
	bloodtypes = []
	for gene in genes:
		
		# Determine base of blood type
		if gene.find('AA') != -1 or gene.find('AO') != -1 or gene.find('OA') != -1:
			bt = 'A'
		elif gene.find('BB') != -1 or gene.find('BO') != -1 or gene.find('OB') != -1:
			bt = 'B'
		elif gene.find('AB') != -1 or gene.find('BA') != -1:
			bt = 'AB'
		elif gene.find('OO') != -1:
			bt = 'O'
		
		# Determine rh of blood type
		if gene.find('++') != -1 or gene.find('+-') != -1 or gene.find('-+') != -1:
			bt = bt + '+'
		else:
			bt = bt + '-'
	
		bloodtypes.append(bt)

	# Remove duplicates
	return sorted(list(set(bloodtypes)))

for line in sys.stdin:

	# Read line
	bloodtypes = line.rstrip().upper().split(' ')

	# Find possible genes for each blood-type
	genes = []
	unknown = -1
	for n in range(0, len(bloodtypes)):
		bt = bloodtypes[n]
		if bt == '?':
			unknown = n
			genes.append([])
		else:
			genes.append(get_genes_for_bloodtype(bt))

	if unknown == 0 or unknown == 1:
		# Parent is unknown
		genes[unknown] = find_parent_genes(genes[(unknown + 1) % 2], genes[2])
	else:
		# Child is unknown
		genes[2] = find_child_genes(genes[0], genes[1])

	# Convert genes into arrays of possible blood-types
	bloodtypes = []
	for c in genes:
		bloodtypes.append(get_bloodtypes_from_genes(c))

	# Prepare output
	output = line.rstrip() + ": "
	for bt in bloodtypes:
		if len(bt) == 0:
			output = output + "IMPOSSIBLE "
		elif len(bt) == 1:
			output = output + bt[0] + " "
		else:
			output = output + str(bt) + " "

	print output
