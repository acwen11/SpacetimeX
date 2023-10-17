#include <cctk.h>
#include <cctk_Arguments.h>
#include <cctk_Parameters.h>
#include <util_Table.h>
#include <loop_device.hxx>
#include <mpi.h>

#include <cassert>
#include <cmath>
#include <cstdio>
#include <array>
#include <ctype.h>

namespace PunctureTracker {
using namespace std;
using namespace Loop;

const int max_num_tracked = 10;

extern "C" void PunctureTracker_Init(CCTK_ARGUMENTS) {
  DECLARE_CCTK_ARGUMENTS_PunctureTracker_Init;
  DECLARE_CCTK_PARAMETERS;

  if (verbose) {
    CCTK_INFO("Initializing PunctureTracker");
  }

  for (int n = 0; n < max_num_tracked; ++n) {
    if (track[n]) {
      pt_loc_t[n] = cctk_time;
      pt_loc_x[n] = initial_x[n];
      pt_loc_y[n] = initial_y[n];
      pt_loc_z[n] = initial_z[n];
      pt_vel_t[n] = cctk_time;
      pt_vel_x[n] = 0.0;
      pt_vel_y[n] = 0.0;
      pt_vel_z[n] = 0.0;
    } else {
      // Initialise to some sensible but unimportant values
      pt_loc_t[n] = 0.0;
      pt_loc_x[n] = 0.0;
      pt_loc_y[n] = 0.0;
      pt_loc_z[n] = 0.0;
      pt_vel_t[n] = 0.0;
      pt_vel_x[n] = 0.0;
      pt_vel_y[n] = 0.0;
      pt_vel_z[n] = 0.0;
    }
  }
	
	// enabled if refinement regions should follow the punctures
	if (track_boxes) {
		const int max_num_regions = 2;
		for (int i = 0; i < max_num_regions; i++) {
			CCTK_VINFO("Writing punc coords to box %d.", i);
			position_x[i] = pt_loc_x[i];
			position_y[i] = pt_loc_y[i];
			position_z[i] = pt_loc_z[i];
		}
	}
}

extern "C" void PunctureTracker_Track(CCTK_ARGUMENTS) {
  DECLARE_CCTK_ARGUMENTS_PunctureTracker_Track;
  DECLARE_CCTK_PARAMETERS;

  // Do not track while setting up initial data;
  // time interpolation may fail

  if (cctk_iteration == 0) {
    return;
  }

  // Some output

  if (verbose) {
    CCTK_INFO("Tracking punctures...");
  }

  if (verbose) {
    for (int n = 0; n < max_num_tracked; ++n) {
      if (track[n]) {
        CCTK_VINFO("Puncture #%d is at (%g,%g,%g)", n, double(pt_loc_x[n]),
                   double(pt_loc_y[n]), double(pt_loc_z[n]));
      }
    }
  }

  // Manual time level cycling
	CCTK_REAL pt_t_prev[max_num_tracked];

  for (int n = 0; n < max_num_tracked; ++n) {
    if (track[n]) {
			pt_t_prev[n] = pt_loc_t[n];
      pt_loc_t[n] = cctk_time;
      pt_vel_t[n] = cctk_time;
    }
		else {
			pt_t_prev[n] = 0.0;
		}
  }

	// Force circular path for debugging
	if (force_circular_path) {
		pt_loc_x[0] = 0.3 * cos((2 * M_PI / 20) * cctk_iteration);
		pt_loc_y[0] = 0.3 * sin((2 * M_PI / 20) * cctk_iteration);
		pt_loc_z[0] = 0;

		if (track_boxes) {
			position_x[0] = pt_loc_x[0];
			position_y[0] = pt_loc_y[0];
			position_z[0] = pt_loc_z[0];
		}
		return;
	}
}

} //namespace PunctureTracker
