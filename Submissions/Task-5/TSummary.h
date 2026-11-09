#pragma once
// For statistics
typedef struct _TSummary {
	long comparisonCount = 0;
	double timeTaken = 0.0;
}TSummary;


inline std::ostream& operator<<(std::ostream& os, const TSummary& summary)
{
	os << "Comparisons: " << summary.comparisonCount << ", Time taken: " << summary.timeTaken << " seconds." << std::endl;
	return os;
}
