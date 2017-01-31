import sys
# If stressed, exit with 1, else exit with 0

# Feature list
# 1. Mean of RR intervals (IBI)
# 2. Standard deviation of RR Intervals (IBI)
# 3. The mean heart rate (BVP)
# 4. Standard deviation of instantaneous heart rate values (BVP)
# 5. Mean level of skin conductance (GSR)
# 6. Standard deviation of skin conductance amplitude (GSR)
# 7. Median of skin conductance amplitude (GSR)
# 8. Mean of Blood oxygen level (BO)
# 9. Blood oxygen level variation (BO)
# 10. Mean of systolic blood pressure (BPM)
# 11. Variance of systolic blood pressure (BPM)
# 12. Mean of diastolic blood pressure (BPM)
# 13. Variance of diastolic blood pressure (BPM)

stressed = False
if stressed:
	sys.exit(1)
else:
	sys.exit(0)