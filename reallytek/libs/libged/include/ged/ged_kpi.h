#ifndef __GPU_KPI_H
#define __GPU_KPI_H

#include "ged.h"

GED_KPI_HANDLE ged_kpi_create(void);
void ged_kpi_destroy(GED_KPI_HANDLE hKPI);
GED_ERROR ged_kpi_record(GED_KPI_HANDLE hKPI, void *win, int fence, int frameID);

#endif
