/*****************************************************************************
 * SV_SplitRead.h
 * (c) 2012 - Ryan M. Layer
 * Hall Laboratory
 * Quinlan Laboratory
 * Department of Computer Science
 * Department of Biochemistry and Molecular Genetics
 * Department of Public Health Sciences and Center for Public Health Genomics,
 * University of Virginia
 * rl6sf@virginia.edu
 *
 * Licenced under the GNU General Public License 2.0 license.
 * ***************************************************************************/

#ifndef __SV_SPLIT_READ_H__
#define __SV_SPLIT_READ_H__

//#include "BamAncillary.h"
using namespace BamTools;

#include "SV_Evidence.h"
#include "SV_BreakPoint.h"
#include "ucsc_bins.hpp"
#include "log_space.h"
#include "bedFilePE.h"

#include <iostream>

using namespace std;

struct cigar_query {
	int qs_pos, qe_pos, q_len;	
};

class SV_SplitRead: public SV_Evidence
{
	friend ostream& operator<<(ostream& out, const SV_SplitRead& p);

	private:

		//void set_bp_interval_probability(struct breakpoint_interval *i);
		static void set_bp_interval_start_end(struct breakpoint_interval *i,
											  struct interval *target_interval,
											  struct interval *target_pair);

		static struct cigar_query calc_query_pos_from_cigar(
					vector< CigarOp > cigar_data,
					bool is_reverse_strand);

		static void update_cigar_query(CigarOp op,
									   int *op_position,
									   struct cigar_query *q);

	public:
		SV_SplitRead(vector< BamAlignment > &block,
					 const RefVector &refs,
					 int weight,
					 int id,
					 int sample_id);

		SV_SplitRead(const BamAlignment &bam_a,
					 const BamAlignment &bam_b,
					 const RefVector &refs,
					 int _weight,
					 int _id,
					 int _sample_id);

		static int back_distance;
		static int min_non_overlap;
		static int min_mapping_threshold;
		static int min_split_size;
		static int read_length;

		struct interval side_l, side_r;
		struct cigar_query query_l, query_r;
		int min_mapping_quality;

		static log_space* get_bp_interval_probability(char strand);

		SV_BreakPoint* get_bp();

		void print_evidence();
		void print_bedpe(int score);

		bool is_sane();

		static void process_split(const BamAlignment &curr,
								  const RefVector refs,
								  map<string, BamAlignment> &mapped_splits,
								  UCSCBins<SV_BreakPoint*> &r_bin,
								  int weight,
								  int id,
								  int sample_id);
};

#endif
