# Ahmed Musse
#
# classify.py
#
# This file contains the code used to build the machine learning model and run the 
# classification algorithm
import sys
import os
import csv
import threading
import time
import pickle
import numpy as np
from sklearn.svm import SVC
from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import accuracy_score, classification_report, confusion_matrix
from sklearn.model_selection import GridSearchCV
from sklearn.decomposition import PCA
from sklearn.preprocessing import normalize
from sklearn.feature_selection import SelectFromModel, chi2, SelectKBest, mutual_info_classif, VarianceThreshold
from scipy import stats
from hrv import classical

# Classification
STRESSED 				= 1
NOT_STRESSED 			= 0

# Model to run
TRAINING 				= 0
TESTING 				= 1

# Total Number of features = 60

# Indicies of BO features
BO_FEATURES_START 		= 0
BO_FEATURES_END 		= 6

# Indicies of IBI features
IBI_FEATURES_START 		= 6
IBI_FEATURES_END 		= 42

# Indicies of GSR features
GSR_FEATURES_START 		= 42
GSR_FEATURES_END 		= 54

# Indicies of TEMP features
TEMP_FEATURES_START 	= 54
TEMP_FEATURES_END 		= 56

# Indicies of BP features
BP_FEATURES_START 		= 56
BP_FEATURES_END 		= 60

# This function reads the features derived form Ledalab from the file "filename"
def get_ledalab_features(filename):
	with open(filename) as file:
		data = []
		count = 0
		for line in file:
			if count > 0:
				data.append([col.strip() for col in line.split('\t')][1:-2])
			count = count + 1

		# Extract only first 21 windows (samples)
		data = np.array(data[:21])
	return data

# This function reads a .txt and puts it into matrix format
def read_txt_file(filename):
	matrix = []
	with open(filename) as file:
		line_count = 0
		beginning_time = 0
		for line in file:
			line = line.rstrip()
			row = line.split(' ')
			if line_count == 0:
				beginning_time = float(row[0])
				row[0] = 0.0
			else:
				row[0] = float(row[0]) - beginning_time
			matrix.append(row)
			line_count = line_count + 1
	return np.array(matrix)

# This function splits the provided matrix and targets into training, validation, and test sets
def split_train_val_test(matrix, targets, part):
	total_samples = len(matrix) 

	# Number of phases in data set
	num_phases = 8
	num_features = 60

	if part == 'part1':
		# For first 8 phases
		samples_per_phase 				= 17
		training_samples_per_phase 		= 9
		validation_samples_per_phase 	= 4
		test_samples_per_phase 			= 4

		total_train_samples = training_samples_per_phase * num_phases
		total_validation_samples = validation_samples_per_phase * num_phases
		total_test_samples = test_samples_per_phase * num_phases
	elif part == 'part2':
		# For last 8 phases
		samples_per_phase 				= 5
		training_samples_per_phase 		= 3
		validation_samples_per_phase 	= 1
		test_samples_per_phase 			= 1
		total_train_samples = training_samples_per_phase * num_phases
		total_validation_samples = validation_samples_per_phase * num_phases
		total_test_samples = test_samples_per_phase * num_phases

	train_matrix = np.zeros((total_train_samples, num_features)) 
	train_targets = np.zeros(total_train_samples)

	validation_matrix = np.zeros((total_validation_samples, num_features)) 
	validation_targets = np.zeros(total_validation_samples)

	test_matrix = np.zeros((total_test_samples, num_features)) 
	test_targets = np.zeros(total_test_samples)

	matrix_start_index = 0
	train_start_index = 0
	val_start_index = 0
	test_start_index = 0

	for i in range(0, num_phases):
		train_end_index = train_start_index + training_samples_per_phase
		matrix_end_index = matrix_start_index + training_samples_per_phase
		train_matrix[train_start_index:train_end_index, :] = matrix[matrix_start_index:matrix_end_index , :]
		train_targets[train_start_index:train_end_index] = targets[matrix_start_index:matrix_end_index]

		train_start_index = train_start_index + training_samples_per_phase
		matrix_start_index = matrix_start_index + training_samples_per_phase + 2

		val_end_index = val_start_index + validation_samples_per_phase
		matrix_end_index = matrix_start_index + validation_samples_per_phase
		validation_matrix[val_start_index:val_end_index, :] = matrix[matrix_start_index:matrix_end_index, :]
		validation_targets[val_start_index:val_end_index] = targets[matrix_start_index:matrix_end_index]

		val_start_index = val_start_index + validation_samples_per_phase
		matrix_start_index = matrix_start_index + validation_samples_per_phase + 2

		test_end_index = test_start_index + test_samples_per_phase
		matrix_end_index = matrix_start_index + test_samples_per_phase
		test_matrix[test_start_index:test_end_index, :]  = matrix[matrix_start_index:matrix_end_index, :]
		test_targets[test_start_index:test_end_index] = targets[matrix_start_index:matrix_end_index]

		test_start_index = test_start_index + test_samples_per_phase
		matrix_start_index = matrix_start_index + test_samples_per_phase

	return train_matrix, train_targets, validation_matrix, validation_targets, test_matrix, test_targets

# This function performs SelectKBest feature selection using the chi2 scoring function and transforms
# the provided matricies to the desired dimensions
def feature_selection_chi2(train_matrix, validation_matrix, test_matrix, train_targets, k_best):
	k = SelectKBest(chi2, k=k_best)
	train_matrix = k.fit_transform(train_matrix, train_targets)
	validation_matrix = k.transform(validation_matrix)
	test_matrix = k.transform(test_matrix)
	num_features = len(validation_matrix[0])
	return train_matrix, validation_matrix, test_matrix, num_features

# This function performs Principal Component Analysis on the provided matricies and transforms
# them.s
def perform_pca(train_matrix, validation_matrix, test_matrix, components):
	train_matrix = normalize(train_matrix)
	validation_matrix = normalize(validation_matrix)
	test_matrix = normalize(test_matrix)
	pca = PCA(n_components=components)
	train_matrix = pca.fit_transform(train_matrix)
	validation_matrix = pca.transform(validation_matrix)
	test_matrix = pca.transform(test_matrix)
	num_features = len(validation_matrix[0])
	return train_matrix, validation_matrix, test_matrix, num_features

# This function performs variance thresholding on the provided matricies and transforms
# them
def perform_thresholding(train_matrix, validation_matrix, test_matrix, t):
	vt = VarianceThreshold(threshold=t)
	train_matrix = vt.fit_transform(train_matrix)
	validation_matrix = vt.transform(validation_matrix)
	test_matrix = vt.transform(test_matrix)
	num_features = len(validation_matrix[0])
	return train_matrix, validation_matrix, test_matrix, num_features

# This function extracts blood oxygen features from the file "filename" and
# returns a matrix with the list of features 
def derive_bo_features(filename, t_lapse, total_samples):
	bo_features = read_txt_file(filename)
	times = bo_features[:,0]

	# Index into times array where new window starts
	start_index = 0

	# Time where window starts
	start_time = t_lapse

	# Time where window ends
	window_end_time = 30.0

	# Number of samples
	num_samples = 0

	# Number of measurements in phase
	num_measurements = len(bo_features)

	features = np.zeros((total_samples, 6))
	while num_samples < total_samples:
		found_start = False
		for index in range(start_index, num_measurements):
			measurement_time = float(times[index])
			if not found_start:
				if measurement_time >= start_time:
					found_start = True
					next_start_index = index
			if measurement_time >= window_end_time:
				break

		# [timestamp, blood_oxygen, pulse_intensity, beats_per_minute]
		samples = bo_features[start_index:index, :]

		blood_oxygen = samples[:, 1].astype(float)
		pulse_intensity = samples[:, 2].astype(float)
		beats_per_minute = samples[:, 3].astype(float)

		# Remove values with 0
		remove_indicies = []
		for i in range(0, len(blood_oxygen)):
			if blood_oxygen[i] == 0.0:
				remove_indicies.append(i)

		blood_oxygen = np.delete(blood_oxygen, remove_indicies)
		pulse_intensity = np.delete(pulse_intensity, remove_indicies)
		beats_per_minute = np.delete(beats_per_minute, remove_indicies)

		# Process features for this sample
		blood_oxygen_mean = np.mean(blood_oxygen)
		blood_oxygen_variance = np.var(blood_oxygen)

		pulse_intensity_mean = np.mean(pulse_intensity)
		pulse_intensity_variance = np.var(pulse_intensity)

		beats_per_minute_mean = np.mean(beats_per_minute)
		beats_per_minute_variance = np.var(beats_per_minute)

		features[num_samples, 0] = blood_oxygen_mean
		features[num_samples, 1] = blood_oxygen_variance

		features[num_samples, 2] = pulse_intensity_mean
		features[num_samples, 3] = pulse_intensity_variance

		features[num_samples, 4] = beats_per_minute_mean
		features[num_samples, 5] = beats_per_minute_variance

		# Increment window logic
		window_end_time = window_end_time + t_lapse
		start_time = start_time + t_lapse
		start_index = next_start_index
		num_samples = num_samples + 1

	return features

# This function extracts the IBI and negative IBI features from the file "filename" and returns
# a matrix with the list of features
def derive_ibi_features(filename, nibi_features_file, t_lapse, total_samples):
	ibi_features = read_txt_file(filename)
	nibi_features = read_txt_file(nibi_features_file)

	times = ibi_features[:,0]
	ntimes = nibi_features[:,0]

	# Number of features
	num_features = 13 + 3 + 2 + 13 + 3 + 2

	# Number of measurements in phase
	num_measurements = len(ibi_features)
	nnum_measurements = len(nibi_features)

	# Index into times array where new window starts
	start_index = 0

	# Time where window starts
	start_time = t_lapse

	# Time where window ends
	window_end_time = 30.0

	# Number of samples
	num_samples = 0

	features = np.zeros((total_samples, num_features))
	while num_samples < total_samples:
		found_start = False
		for index in range(start_index, num_measurements):
			measurement_time = float(times[index])
			if not found_start:
				if measurement_time >= start_time:
					found_start = True
					next_start_index = index
			if measurement_time >= window_end_time:
				break

		# [timestamp, ibi]
		samples = ibi_features[start_index:index, :]

		ibi = samples[:, 1].astype(float)
		mags = samples[:, 2].astype(float)

		# Process features for this sample
		# 6 features
		time_domain_features = classical.time_domain(ibi) 
		# 7 features
		frequency_domain_features = classical.frequency_domain(ibi, method='welch', 
			interp_freq=4, segment_size=len(ibi), overlap_size=50, window_function='hann') 

		features[num_samples, 0] = time_domain_features['sdnn']
		features[num_samples, 1] = time_domain_features['mrri']
		features[num_samples, 2] = time_domain_features['pnn50']
		features[num_samples, 3] = time_domain_features['mhr']
		features[num_samples, 4] = time_domain_features['rmssd']
		features[num_samples, 5] = time_domain_features['nn50']

		features[num_samples, 6] = frequency_domain_features['lf']
		features[num_samples, 7] = frequency_domain_features['lfnu']
		features[num_samples, 8] = frequency_domain_features['lf_hf']
		features[num_samples, 9] = frequency_domain_features['total_power']
		features[num_samples, 10] = frequency_domain_features['hfnu']
		features[num_samples, 11] = frequency_domain_features['vlf']
		features[num_samples, 12] = frequency_domain_features['hf']

		features[num_samples, 13] = np.percentile(ibi, 10)
		features[num_samples, 14] = np.percentile(ibi, 50)
		features[num_samples, 15] = np.percentile(ibi, 90)

		features[num_samples, 16] = np.percentile(mags, 10)
		features[num_samples, 17] = np.percentile(mags, 80)

		# Increment window logic
		window_end_time = window_end_time + t_lapse
		start_time = start_time + t_lapse
		start_index = next_start_index
		num_samples = num_samples + 1

	# Index into times array where new window starts
	start_index = 0

	# Time where window starts
	start_time = t_lapse

	# Time where window ends
	window_end_time = 30.0

	# Number of samples
	num_samples = 0

	while num_samples < total_samples:
		found_start = False
		for index in range(start_index, nnum_measurements):
			measurement_time = float(ntimes[index])
			if not found_start:
				if measurement_time >= start_time:
					found_start = True
					next_start_index = index
			if measurement_time >= window_end_time:
				break

		# [timestamp, ibi]
		samples = nibi_features[start_index:index, :]

		ibi = samples[:, 1].astype(float)
		mags = samples[:, 2].astype(float)

		# Process features for this sample
		# 6 features
		time_domain_features = classical.time_domain(ibi) 
		# 7 features
		frequency_domain_features = classical.frequency_domain(ibi, method='welch', 
			interp_freq=4, segment_size=len(ibi), overlap_size=50, window_function='hann') 

		features[num_samples, 18] = time_domain_features['sdnn']
		features[num_samples, 19] = time_domain_features['mrri']
		features[num_samples, 20] = time_domain_features['pnn50']
		features[num_samples, 21] = time_domain_features['mhr']
		features[num_samples, 22] = time_domain_features['rmssd']
		features[num_samples, 23] = time_domain_features['nn50']

		features[num_samples, 24] = frequency_domain_features['lf']
		features[num_samples, 25] = frequency_domain_features['lfnu']
		features[num_samples, 26] = frequency_domain_features['lf_hf']
		features[num_samples, 27] = frequency_domain_features['total_power']
		features[num_samples, 28] = frequency_domain_features['hfnu']
		features[num_samples, 29] = frequency_domain_features['vlf']
		features[num_samples, 30] = frequency_domain_features['hf']

		features[num_samples, 31] = np.percentile(ibi, 10)
		features[num_samples, 32] = np.percentile(ibi, 50)
		features[num_samples, 33] = np.percentile(ibi, 90)

		features[num_samples, 34] = np.percentile(mags, 10)
		features[num_samples, 35] = np.percentile(mags, 80)

		# Increment window logic
		window_end_time = window_end_time + t_lapse
		start_time = start_time + t_lapse
		start_index = next_start_index
		num_samples = num_samples + 1

	return features

# This function extracts the GSR features from the file "filename" and returns
# a matrix with the list of features
def derive_gsr_features(filename, t_lapse, total_samples):
	cda_features = get_ledalab_features(filename)
	return cda_features

# This function extracts the skin temperature features from the file "filename" and returns
# a matrix with the list of features
def derive_temp_features(filename, t_lapse, total_samples):
	temp_features = read_txt_file(filename)
	times = temp_features[:,0]

	# Index into times array where new window starts
	start_index = 0

	# Time where window starts
	start_time = t_lapse

	# Time where window ends
	window_end_time = 30.0

	# Number of samples
	num_samples = 0

	# Number of features
	num_features = 2

	# Number of measurements in phase
	num_measurements = len(temp_features)

	features = np.zeros((total_samples, num_features))
	while num_samples < total_samples:
		found_start = False
		for index in range(start_index, num_measurements):
			measurement_time = float(times[index])
			if not found_start:
				if measurement_time >= start_time:
					found_start = True
					next_start_index = index
			if measurement_time >= window_end_time:
				break

		# [timestamp, blood_oxygen, pulse_intensity, beats_per_minute]
		samples = temp_features[start_index:index, :]

		temps = samples[:, 1].astype(float)

		# Process features for this sample
		temps_mean = np.mean(temps)
		temps_variance = np.var(temps)

		features[num_samples, 0] = temps_mean
		features[num_samples, 1] = temps_variance

		# Increment window logic
		window_end_time = window_end_time + t_lapse
		start_time = start_time + t_lapse
		start_index = next_start_index
		num_samples = num_samples + 1
	return features

# This function extracts the blood pressure features from the file "filename" and returns
# a matrix with the list of features
def derive_bp_features(filename, t_lapse, total_samples):
	bp_features = read_txt_file(filename)
	times = bp_features[:,0]

	# Index into times array where new window starts
	start_index = 0

	# Time where window starts
	start_time = t_lapse

	# Time where window ends
	window_end_time = 30.0

	# Number of samples
	num_samples = total_samples

	# Number of features
	num_features = 4

	# Number of measurements in phase
	num_measurements = len(bp_features)

	features = np.zeros((total_samples, num_features))

	sys = bp_features[:, 1].astype(float)
	dia = bp_features[:, 2].astype(float)

	sys_mean = np.mean(sys)
	sys_var = np.var(sys)

	dia_mean = np.mean(dia)
	dia_var = np.var(dia)

	features[:, 0] = sys_mean
	features[:, 1] = sys_var

	features[:, 2] = dia_mean
	features[:, 3] = dia_var

	return features	

# This function creates the feature matrix for the test subject data defined by path "path".
# It only builds the feature matrix using the first 8 phases of the experiment
def create_feature_matrix(path):
	# Window size in seconds
	window_size = 30.0

	# Time in-between windows in seconds
	t_lapse = 10.0

	# Phase length is 4 minutes in seconds
	phase_length = 4 * 60 

	# Number of total phases
	num_phases = 8

	# Total number of samples
	total_num_samples = int((phase_length - window_size)/t_lapse) * num_phases

	# Samples per phase
	samples_per_phase = int((phase_length - window_size)/t_lapse)

	# Number of features in feature matrix
	num_features = 6 + 13 + 12 + 2 + 4 + 10 + 13

	matrix = np.zeros((total_num_samples, num_features))
	targets = np.zeros(total_num_samples)

	# Sample index
	sample_index_start = 0
	sample_index_end = samples_per_phase

	for i in range(1, num_phases + 1):
		phase_number = str(i)
		bo_features_file = path + '/BO/BO_data_phase' + phase_number + '.txt'
		bp_features_file = path + '/BP/BP_data_phase' + phase_number + '.txt'
		bvp_features_file = path + '/BVP/BVP_data_phase' + phase_number + '.csv'
		temp_features_file = path + '/TEMP/TEMP_data_phase_' + phase_number + '.txt'
		ibi_features_file = path + '/IBI/IBI_data_phase' + phase_number + '.txt'
		nibi_features_file = path + '/IBI/NIBI_data_phase' + phase_number + '.txt'
		gsr_features_file = path + '/GSR/GSR_data_phase' + phase_number + '_cda.txt'

		bo_features = derive_bo_features(bo_features_file, t_lapse, samples_per_phase)
		matrix[sample_index_start:sample_index_end, BO_FEATURES_START:BO_FEATURES_END] = \
		bo_features[:,:]

		ibi_features = derive_ibi_features(ibi_features_file, nibi_features_file, t_lapse, samples_per_phase)
		matrix[sample_index_start:sample_index_end, IBI_FEATURES_START:IBI_FEATURES_END] = \
		ibi_features[:,:]

		gsr_features = derive_gsr_features(gsr_features_file, t_lapse, samples_per_phase)
		matrix[sample_index_start:sample_index_end, GSR_FEATURES_START:GSR_FEATURES_END] = \
		gsr_features[:,:]

		temp_features = derive_temp_features(temp_features_file, t_lapse, samples_per_phase)
		matrix[sample_index_start:sample_index_end, TEMP_FEATURES_START:TEMP_FEATURES_END] = \
		temp_features[:,:]

		bp_features = derive_bp_features(bp_features_file, t_lapse, samples_per_phase)
		matrix[sample_index_start:sample_index_end, BP_FEATURES_START:BP_FEATURES_END] = \
		bp_features[:,:]

		# If even phase, under stress, classify as stressed
		if i % 2 == 0:
			classification = STRESSED
			targets[sample_index_start:sample_index_end] = np.ones(samples_per_phase, dtype=np.int)
		# If odd phase, relaxing, classify as not stressed
		else:
			classification = NOT_STRESSED
			targets[sample_index_start:sample_index_end] = np.zeros(samples_per_phase, dtype=np.int)

		sample_index_start = sample_index_end
		sample_index_end = sample_index_end + samples_per_phase

	return np.nan_to_num(matrix), targets.astype(int)

# This function creates the feature matrix for the test subject data defined by path "path".
# It only builds the feature matrix using the last 8 phases of the experiment
def create_feature_matrix_part2(path):
	# Window size in seconds
	window_size = 30.0

	# Time in-between windows in seconds
	t_lapse = 10.0

	# Phase length is 2 minutes in seconds
	phase_length = 2 * 60 

	# Number of total phases
	num_phases = 8

	# Samples per phase
	samples_per_phase = int((phase_length - window_size)/t_lapse) # 9

	# Total number of samples
	total_num_samples = samples_per_phase * num_phases # 72

	# Number of features in feature matrix
	num_features = 6 + 13 + 12 + 2 + 4 + 10 + 13

	matrix = np.zeros((total_num_samples, num_features))
	targets = np.zeros(total_num_samples)

	# Sample index
	sample_index_start = 0
	sample_index_end = samples_per_phase

	for i in range(9, 9 + num_phases):
		phase_number = str(i)
		bo_features_file = path + '/BO/BO_data_phase' + phase_number + '.txt'
		bp_features_file = path + '/BP/BP_data_phase' + phase_number + '.txt'
		bvp_features_file = path + '/BVP/BVP_data_phase' + phase_number + '.csv'
		temp_features_file = path + '/TEMP/TEMP_data_phase_' + phase_number + '.txt'
		ibi_features_file = path + '/IBI/IBI_data_phase' + phase_number + '.txt'
		nibi_features_file = path + '/IBI/NIBI_data_phase' + phase_number + '.txt'
		gsr_features_file = path + '/GSR/GSR_data_phase' + phase_number + '_cda.txt'

		bo_features = derive_bo_features(bo_features_file, t_lapse, samples_per_phase)
		matrix[sample_index_start:sample_index_end, BO_FEATURES_START:BO_FEATURES_END] = \
		bo_features[:samples_per_phase,:]

		ibi_features = derive_ibi_features(ibi_features_file, nibi_features_file, t_lapse, samples_per_phase)
		matrix[sample_index_start:sample_index_end, IBI_FEATURES_START:IBI_FEATURES_END] = \
		ibi_features[:samples_per_phase,:]

		gsr_features = derive_gsr_features(gsr_features_file, t_lapse, samples_per_phase)
		matrix[sample_index_start:sample_index_end, GSR_FEATURES_START:GSR_FEATURES_END] = \
		gsr_features[:samples_per_phase,:]

		temp_features = derive_temp_features(temp_features_file, t_lapse, samples_per_phase)
		matrix[sample_index_start:sample_index_end, TEMP_FEATURES_START:TEMP_FEATURES_END] = \
		temp_features[:samples_per_phase,:]

		bp_features = derive_bp_features(bp_features_file, t_lapse, samples_per_phase)
		matrix[sample_index_start:sample_index_end, BP_FEATURES_START:BP_FEATURES_END] = \
		bp_features[:samples_per_phase,:]

		# If even phase, under stress, classify as stressed
		if i % 2 == 0:
			classification = STRESSED
			targets[sample_index_start:sample_index_end] = np.ones(samples_per_phase, dtype=np.int)
		# If odd phase, relaxing, classify as not stressed
		else:
			classification = NOT_STRESSED
			targets[sample_index_start:sample_index_end] = np.zeros(samples_per_phase, dtype=np.int)

		sample_index_start = sample_index_end
		sample_index_end = sample_index_end + samples_per_phase

	return np.nan_to_num(matrix), targets.astype(int)

# This function tests the individual model
def test_individual_model():
	svm_accuracies = np.zeros(0)
	nn5_accuracies = np.zeros(0)

	svm_val_accuracies = np.zeros(0)
	svm_testw_accuracies = np.zeros(0)

	nn5_val_accuracies = np.zeros(0)
	nn5_testw_accuracies = np.zeros(0)

	start_test = 1
	end_test = 7

	for i in range(start_test, end_test + 1):
		path = './src/tests/test0' + str(i)
		print "==========================TEST ", i, "================================"
		matrix_part1, targets_part1 = create_feature_matrix(path)
		matrix_part2, targets_part2 = create_feature_matrix_part2(path)

		train_matrix_part1, train_targets_part1, validation_matrix_part1, \
		validation_targets_part1, test_matrix_part1, test_targets_part1 = \
		split_train_val_test(matrix_part1, targets_part1, 'part1')
		train_matrix_part2, train_targets_part2, validation_matrix_part2, \
		validation_targets_part2, test_matrix_part2, test_targets_part2 = \
		split_train_val_test(matrix_part2, targets_part2, 'part2')

		train_matrix = np.vstack((train_matrix_part1, train_matrix_part2))
		train_targets = np.append(train_targets_part1, train_targets_part2)

		validation_matrix = np.vstack((validation_matrix_part1, validation_matrix_part2))
		validation_targets = np.append(validation_targets_part1, validation_targets_part2)

		test_matrix = np.vstack((test_matrix_part1, test_matrix_part2))
		test_targets = np.append(test_targets_part1, test_targets_part2)

		# Remove negative entries from feature matrix
		row_index = 0
		for row in train_matrix:
			col_index = 0
			for col in row:
				if col < 0:
					train_matrix[row_index, col_index] = abs(col)
				col_index = col_index + 1
			row_index = row_index + 1

		k = 40
		components = 10
		threshold = 10.0

		train_matrix, validation_matrix, test_matrix, num_features = \
		feature_selection_chi2(train_matrix, validation_matrix, test_matrix, train_targets, k)
		train_matrix, validation_matrix, test_matrix, num_features = \
		perform_pca(train_matrix, validation_matrix, test_matrix, components)
		train_matrix, validation_matrix, test_matrix, num_features = \
		perform_thresholding(train_matrix, validation_matrix, test_matrix, threshold)

		clf = SVC(kernel='linear')
		clf.fit(train_matrix, train_targets)
		y_val = clf.predict(validation_matrix)
		y_test = clf.predict(test_matrix)

		validation_accuracy  = accuracy_score(validation_targets, y_val)
		test_accuracy = accuracy_score(test_targets, y_test)
		print "SVM Linear Kernel Validation Accuracy: " + str(validation_accuracy)
		print "SVM Linear Kernel Test Accuracy: " + str(test_accuracy)
		svm_val_accuracies = np.append(svm_val_accuracies, validation_accuracy)

		train_matrix = np.vstack((train_matrix, validation_matrix))
		train_targets = np.append(train_targets, validation_targets)
		clf = SVC(kernel='linear')
		clf.fit(train_matrix, train_targets)
		y_test = clf.predict(test_matrix)

		test_accuracy = accuracy_score(test_targets, y_test)
		svm_testw_accuracies = np.append(svm_testw_accuracies, test_accuracy)
		print "SVM Linear Kernel Test Accuracy w/Val: " + str(test_accuracy)

	print "Average SMV Linear Kernel Test Accuracy: ", str(np.mean(svm_accuracies))
	print "Average SVM Linear Validation Accuracy: ", str(np.mean(svm_val_accuracies))
	print "Average SVM Linear Testw Accuracy: ", str(np.mean(svm_testw_accuracies))




# This function combines all training sets to build general model
def build_general_model():
	path = "./src/tests/"
	
	start_test = 3
	end_test = 6
	num_tests = end_test - start_test + 1
	num_features = 6 + 13 + 12 + 2 + 4 + 10 + 13

	general_train_matrix = np.zeros((0, num_features - 23))
	general_train_targets = np.zeros(1, dtype=int)

	for i in range(start_test, end_test + 1):
		if i < 10:
			directory = path + "/test0" + str(i)
		else:
			directory = path + "/test" + str(i)

		train_matrix, train_targets = create_feature_matrix(directory)
		
		general_train_matrix = np.vstack((general_train_matrix, train_matrix))
		general_train_targets = np.append(general_train_targets, train_targets)

	general_train_targets = general_train_targets[1:]

	train_matrix = general_train_matrix
	train_targets = general_train_targets

	# Remove negative entries from feature matrix
	row_index = 0
	for row in train_matrix:
		col_index = 0
		for col in row:
			if col < 0:
				train_matrix[row_index, col_index] = abs(col)
			col_index = col_index + 1
		row_index = row_index + 1

	k = 40
	components = 37
	threshold = 10.0

	train_matrix, validation_matrix, test_matrix, num_features = \
	feature_selection_chi2(train_matrix, validation_matrix, test_matrix, train_targets, k)
	train_matrix, validation_matrix, test_matrix, num_features = \
	perform_pca(train_matrix, validation_matrix, test_matrix, components)
	train_matrix, validation_matrix, test_matrix, num_features = \
	perform_thresholding(train_matrix, validation_matrix, test_matrix, threshold)

	clf = SVC(kernel='linear')
	clf.fit(train_matrix, train_targets)

	with open('./data/svml_model.pkl', 'wb') as f:
		pickle.dump(clf, f)

# This function tests the new subjects data on general model
def test_classifier(test_id):
	path = "./src/tests/test" + str(test_id)
	test_matrix, test_targets = create_feature_matrix(path)

	# Load saved models, if they exist
	try:
		clf = pickle.load(open('./data/svml_model.pkl', 'rb'))
	except (OSError, IOError) as e:
		sys.stderr.write('Read Error: Could not read machine learning model\n')
		return NOT_STRESSED

	print "Detailed classification report:"
	y_true, y_pred = test_targets, clf.predict(test_matrix)
	print(classification_report(y_true, y_pred))
	print "Confusion Matrix:"
	print(confusion_matrix(y_true, y_pred))
	print "Accuracy Score:"
	print(accuracy_score(y_true, y_pred))

# This function builds the test set for the classification task
def build_classification_set(path, timestamp):

	# Window size in seconds
	window_size = 30.0

	# Time in-between windows in seconds
	t_lapse = 10.0

	# Phase length is 4 minutes in seconds
	phase_length = 4 * 60 

	# Total number of samples
	total_num_samples = int((phase_length - window_size)/t_lapse)

	# Number of features in feature matrix
	num_features = 6 + 13 + 12 + 2 + 4 + 10 + 13

	test_matrix = np.zeros((total_num_samples, num_features))

	bo_features_file = path + 'BO_data_' + timestamp + '.txt'
	bp_features_file = path + 'BP_data_' + timestamp + '.txt'
	# bvp_features_file = path + 'BVP_data_' + timestamp + '.txt'
	temp_features_file = path + 'TEMP_data_' + timestamp + '.txt'
	ibi_features_file = path + 'IBI_data_' + timestamp + '.txt'
	gsr_features_file = path + 'GSR_data_' + timestamp + '_cda.txt'

	bo_features = derive_bo_features(bo_features_file, t_lapse, total_num_samples)
	test_matrix[:, BO_FEATURES_START:BO_FEATURES_END] = bo_features

	ibi_features = derive_ibi_features(ibi_features_file, t_lapse, total_num_samples)
	test_matrix[:, IBI_FEATURES_START:IBI_FEATURES_END] = ibi_features

	gsr_features = derive_gsr_features(gsr_features_file, t_lapse, total_num_samples)
	test_matrix[:, GSR_FEATURES_START:GSR_FEATURES_END] = gsr_features

	temp_features = derive_temp_features(temp_features_file, t_lapse, total_num_samples)
	test_matrix[:, TEMP_FEATURES_START:TEMP_FEATURES_END] = temp_features[:,:]

	bp_features = derive_bp_features(bp_features_file, t_lapse, total_num_samples)
	test_matrix[:, BP_FEATURES_START:BP_FEATURES_END] = bp_features[:,:]

	return np.nan_to_num(test_matrix)

# This function classifies whether or not the individual is stressed at time "timestamp"
def classify(timestamp):
	# Load saved model, if it exists
	try:
		clf = pickle.load(open('./data/model.pkl', 'rb'))
	except (OSError, IOError) as e:
		sys.stderr.write('Read Error: Could not read machine learning model\n')
		return NOT_STRESSED

	path = "./data/"
	test_matrix = build_classification_set(path, timestamp)
	y_test = clf.predict(test_matrix)
	classification = stats.mode(y_test)
	return classification.mode[0]

# The main function takes command line parameters. This is a list of possible commands
# python classify.py train <test-num>
# python classify.py classify <unix-timestamp>
# python classify.py test_general <test-num>
# python classify.py test_individual
def main():
	np.set_printoptions(suppress=True)
	if len(sys.argv) >=2:
		try:
			operation = str(sys.argv[1])
		except:
			sys.stderr.write('Parameter Error: Error reading parameter\n')
			sys.exit(0)

		if operation.lower() == 'train':
			# Build the general ML model
			if len(sys.argv) < 3:
				sys.stderr.write('Parameter Error: Please supply the test number to analyze\n')
				sys.exit(0)
			try:
				test_id = sys.argv[2]
			except:
				sys.stderr.write('Parameter Error: Error reading parameter\n')
				sys.exit(0)
			build_general_model()
			test_classifier(test_id)
			sys.exit(0)
		elif operation == 'classify':
			if len(sys.argv) < 3:
				sys.stderr.write('Parameter Error: Please supply ' + \
					'the timestamp of the sample to analyze\n')
				sys.exit(0)
			try:
				timestamp = str(sys.argv[2])
			except:
				sys.stderr.write('Parameter Error: Error reading parameter\n')
				sys.exit(0)

			is_stressed = classify(timestamp)
			print is_stressed == STRESSED
			sys.exit(is_stressed)
		elif operation == 'test_general':
			if len(sys.argv) < 3:
				sys.stderr.write('Parameter Error: Please supply the test number to analyze\n')
				sys.exit(0)
			try:
				test_id = sys.argv[2]
			except:
				sys.stderr.write('Parameter Error: Error reading parameter\n')
				sys.exit(0)
			test_classifier(test_id)
		elif operation == 'test_individual':
			test_individual_model()
		else:
			sys.stderr.write('Parameter Error: Please supply a valid parameter\n')
	else:
		sys.stderr.write('Parameter Error: Please specify parameters\n')
		return

main()