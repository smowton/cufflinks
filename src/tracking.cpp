#include "tracking.h"

void add_to_tracking_table(size_t sample_index,
                           Abundance& ab,
						   FPKMTrackingTable& track)

{
	pair<FPKMTrackingTable::iterator,bool> inserted;
	pair<string, FPKMTracking > p;
	p = make_pair(ab.description(), FPKMTracking());
	inserted = track.insert(p);
	
	FPKMTracking& fpkm_track = inserted.first->second;
	
	set<string> tss = ab.tss_id();
    set<string> gene_ids = ab.gene_id();
	set<string> genes = ab.gene_name();
	set<string> proteins = ab.protein_id();
	
	fpkm_track.tss_ids.insert(tss.begin(), tss.end());
    fpkm_track.gene_ids.insert(gene_ids.begin(), gene_ids.end());
	fpkm_track.gene_names.insert(genes.begin(), genes.end());
	fpkm_track.protein_ids.insert(proteins.begin(), proteins.end());
	
	if (inserted.second)
	{
		fpkm_track.locus_tag = ab.locus_tag();
		fpkm_track.description = ab.description();
		shared_ptr<Scaffold> transfrag = ab.transfrag();
		if (transfrag && transfrag->nearest_ref_id() != "")
		{
			fpkm_track.classcode = transfrag->nearest_ref_classcode();
			fpkm_track.ref_match = transfrag->nearest_ref_id();
		}
		else
		{
			fpkm_track.classcode = 0;
			fpkm_track.ref_match = "-";
		}
        if (transfrag)
        {
            fpkm_track.length = transfrag->length();
        }
        else
        {
            fpkm_track.length = 0;
        }
	}
	
	FPKMContext r1 = FPKMContext(ab.num_fragments(),
                                 ab.num_fragment_var(),
                                 ab.num_fragment_uncertainty_var(),
                                 ab.mass_variance(),
                                 ab.num_fragments_by_replicate(),
								 ab.FPKM(),
								 ab.FPKM_by_replicate(),
                                 ab.FPKM_variance(),
                                 ab.FPKM_conf().low,
                                 ab.FPKM_conf().high,
                                 ab.status(),
                                 ab.status_by_replicate(),
                                 ab.fpkm_samples(),
                                 ab.gamma());
    
    
	
    vector<FPKMContext>& fpkms = inserted.first->second.fpkm_series;
    if (sample_index < fpkms.size())
    {
        // if the fpkm series already has an entry matching this description
        // for this sample index, then we are dealing with a group of transcripts
        // that occupies multiple (genomically disjoint) bundles.  We need
        // to add this bundle's contribution to the FPKM, fragments, and variance
        // to whatever's already there.
        
        // NOTE: we can simply sum the FKPM_variances, because we are currently
        // assuming that transcripts in disjoint bundles share no alignments and
        // thus have FPKM covariance == 0;  This assumption will no longer be
        // true if we decide to do multireads the right way.
        
        FPKMContext& existing = fpkms[sample_index];
        existing.FPKM += r1.FPKM;
        existing.count_mean += r1.count_mean;
        existing.FPKM_variance += r1.FPKM_variance;
        if (existing.status == NUMERIC_FAIL || r1.status == NUMERIC_FAIL)
        {
            existing.status = NUMERIC_FAIL;
        }
        else
        {
            existing.status = NUMERIC_OK;
        }
        
    }
    else
    {
        fpkms.push_back(r1);
    }
}


void add_to_allele_tracking_table(size_t sample_index,
                           Abundance& ab,
						   FPKMTrackingTable& track)

{
	pair<FPKMTrackingTable::iterator,bool> inserted;
	pair<string, FPKMTracking > p;
	p = make_pair(ab.description(), FPKMTracking());
	inserted = track.insert(p);
	
	FPKMTracking& fpkm_track = inserted.first->second;
	
	set<string> tss = ab.tss_id();
    set<string> gene_ids = ab.gene_id();
	set<string> genes = ab.gene_name();
	set<string> proteins = ab.protein_id();
	
	fpkm_track.tss_ids.insert(tss.begin(), tss.end());
    fpkm_track.gene_ids.insert(gene_ids.begin(), gene_ids.end());
	fpkm_track.gene_names.insert(genes.begin(), genes.end());
	fpkm_track.protein_ids.insert(proteins.begin(), proteins.end());
	
	if (inserted.second)
	{
		fpkm_track.locus_tag = ab.locus_tag();
		fpkm_track.description = ab.description();
		shared_ptr<Scaffold> transfrag = ab.transfrag();
		if (transfrag && transfrag->nearest_ref_id() != "")
		{
			fpkm_track.classcode = transfrag->nearest_ref_classcode();
			fpkm_track.ref_match = transfrag->nearest_ref_id();
		}
		else
		{
			fpkm_track.classcode = 0;
			fpkm_track.ref_match = "-";
		}
        if (transfrag)
        {
            fpkm_track.length = transfrag->length();
        }
        else
        {
            fpkm_track.length = 0;
        }
	}
	
	FPKMContext paternal_r1 = FPKMContext(ab.num_paternal_fragments(),
                                 ab.num_paternal_fragment_var(),
                                 ab.num_paternal_fragment_uncertainty_var(),
                                 ab.paternal_mass_variance(),
                                 ab.num_paternal_fragments_by_replicate(),
								 ab.paternal_FPKM(),
								 ab.paternal_FPKM_by_replicate(),
                                 ab.paternal_FPKM_variance(),
                                 ab.paternal_FPKM_conf().low,
                                 ab.paternal_FPKM_conf().high,
                                 ab.paternal_status(),
                                 ab.paternal_status_by_replicate(),
                                 ab.paternal_fpkm_samples(),
                                 ab.paternal_gamma());
	FPKMContext maternal_r1 = FPKMContext(ab.num_maternal_fragments(),
                                 ab.num_maternal_fragment_var(),
                                 ab.num_maternal_fragment_uncertainty_var(),
                                 ab.maternal_mass_variance(),
                                 ab.num_maternal_fragments_by_replicate(),
								 ab.maternal_FPKM(),
								 ab.maternal_FPKM_by_replicate(),
                                 ab.maternal_FPKM_variance(),
                                 ab.maternal_FPKM_conf().low,
                                 ab.maternal_FPKM_conf().high,
                                 ab.maternal_status(),
                                 ab.maternal_status_by_replicate(),
                                 ab.maternal_fpkm_samples(),
                                 ab.maternal_gamma());
    
    
	
    vector<FPKMContext>& paternal_fpkms = inserted.first->second.paternal_fpkm_series;
    if (sample_index < paternal_fpkms.size())
    {
        // if the fpkm series already has an entry matching this description
        // for this sample index, then we are dealing with a group of transcripts
        // that occupies multiple (genomically disjoint) bundles.  We need
        // to add this bundle's contribution to the FPKM, fragments, and variance
        // to whatever's already there.
        
        // NOTE: we can simply sum the FKPM_variances, because we are currently
        // assuming that transcripts in disjoint bundles share no alignments and
        // thus have FPKM covariance == 0;  This assumption will no longer be
        // true if we decide to do multireads the right way.
        
        FPKMContext& existing = paternal_fpkms[sample_index];
        existing.FPKM += paternal_r1.FPKM;
        existing.count_mean += paternal_r1.count_mean;
        existing.FPKM_variance += paternal_r1.FPKM_variance;
        if (existing.status == NUMERIC_FAIL || paternal_r1.status == NUMERIC_FAIL)
        {
            existing.status = NUMERIC_FAIL;
        }
        else
        {
            existing.status = NUMERIC_OK;
        }
        
    }
    else
    {
        paternal_fpkms.push_back(paternal_r1);
    }
	//
	vector<FPKMContext>& maternal_fpkms = inserted.first->second.maternal_fpkm_series;
    if (sample_index < maternal_fpkms.size())
    {
        // if the fpkm series already has an entry matching this description
        // for this sample index, then we are dealing with a group of transcripts
        // that occupies multiple (genomically disjoint) bundles.  We need
        // to add this bundle's contribution to the FPKM, fragments, and variance
        // to whatever's already there.
        
        // NOTE: we can simply sum the FKPM_variances, because we are currently
        // assuming that transcripts in disjoint bundles share no alignments and
        // thus have FPKM covariance == 0;  This assumption will no longer be
        // true if we decide to do multireads the right way.
        
        FPKMContext& existing = maternal_fpkms[sample_index];
        existing.FPKM += maternal_r1.FPKM;
        existing.count_mean += maternal_r1.count_mean;
        existing.FPKM_variance += maternal_r1.FPKM_variance;
        if (existing.status == NUMERIC_FAIL || maternal_r1.status == NUMERIC_FAIL)
        {
            existing.status = NUMERIC_FAIL;
        }
        else
        {
            existing.status = NUMERIC_OK;
        }
        
    }
    else
    {
        maternal_fpkms.push_back(maternal_r1);
    }
}
