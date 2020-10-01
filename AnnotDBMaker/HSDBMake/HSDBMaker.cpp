#include "DBMaker.h"
#include "sapp/scuiapp.h"

using namespace slib;
using namespace slib::sbio;

SDictionary hs_gff = {
    kv("ensembl", V({
        kv("gene", "GENE"),
        kv("ncRNA_gene", "GENE"),
        kv("pseudogene", "GENE"),
        
        kv("mRNA", "TRANSCRIPT"),
        kv("tRNA", "TRANSCRIPT"),
        kv("rRNA", "TRANSCRIPT"),
        kv("ncRNA", "TRANSCRIPT"),
        kv("lnc_RNA", "TRANSCRIPT"),
        kv("scRNA", "TRANSCRIPT"),
        kv("snRNA", "TRANSCRIPT"),
        kv("snoRNA", "TRANSCRIPT"),
        kv("pseudogenic_transcript", "TRANSCRIPT"),
        kv("transcript", "TRANSCRIPT"),
        kv("unconfirmed_transcript", "TRANSCRIPT"),
        
        kv("CDS", "STRUCTURE"),
        kv("five_prime_UTR", "STRUCTURE"),
        kv("three_prime_UTR", "STRUCTURE"),
        kv("exon", "STRUCTURE")
    })),
    kv("ensembl_havana", V({
        kv("gene", "GENE"),
        kv("ncRNA_gene", "GENE"),
        kv("bidirectional_promoter_lncRNA", "GENE"),
        kv("pseudogene", "GENE"),
        
        kv("mRNA", "TRANSCRIPT"),
        kv("lnc_RNA", "TRANSCRIPT"),
        kv("C_gene_segment", "TRANSCRIPT"),
        kv("V_gene_segment", "TRANSCRIPT"),
        kv("three_prime_overlapping_ncrna", "TRANSCRIPT"),
        kv("pseudogenic_transcript", "TRANSCRIPT"),
        kv("transcript", "TRANSCRIPT"),
        kv("unconfirmed_transcript", "TRANSCRIPT"),
        
        kv("CDS", "STRUCTURE"),
        kv("five_prime_UTR", "STRUCTURE"),
        kv("three_prime_UTR", "STRUCTURE"),
        kv("exon", "STRUCTURE")
    })),
    
    kv("havana", V({
        kv("gene", "GENE"),
        kv("ncRNA_gene", "GENE"),
        kv("bidirectional_promoter_lncRNA", "GENE"),
        kv("pseudogene", "GENE"),
        
        kv("mRNA", "TRANSCRIPT"),
        kv("lnc_RNA", "TRANSCRIPT"),
        kv("scRNA", "TRANSCRIPT"),
        kv("snRNA", "TRANSCRIPT"),
        kv("snoRNA", "TRANSCRIPT"),
        kv("C_gene_segment", "TRANSCRIPT"),
        kv("D_gene_segment", "TRANSCRIPT"),
        kv("J_gene_segment", "TRANSCRIPT"),
        kv("V_gene_segment", "TRANSCRIPT"),
        kv("three_prime_overlapping_ncrna", "TRANSCRIPT"),
        kv("pseudogenic_transcript", "TRANSCRIPT"),
        kv("transcript", "TRANSCRIPT"),
        kv("unconfirmed_transcript", "TRANSCRIPT"),
        kv("vaultRNA_primary_transcript", "TRANSCRIPT"),
        
        kv("CDS", "STRUCTURE"),
        kv("five_prime_UTR", "STRUCTURE"),
        kv("three_prime_UTR", "STRUCTURE"),
        kv("exon", "STRUCTURE")
    })),
    kv("insdc", V({
        kv("gene", "GENE"),
        kv("ncRNA_gene", "GENE")
    })),
    
    kv("mirbase", V({
        kv("ncRNA_gene", "GENE"),
        
        kv("miRNA", "TRANSCRIPT"),
        
        kv("exon", "STRUCTURE")
    }))
};

void loadGFF_HS(STable *table, const char *path, const sindex &chridx) {
    sbio::SGFFFile gff;
    gff.open(path);
    size_t trs_col_index_g = table[GENE].columnIndex("TRANSCRIPT"),
    strct_col_index_t = table[TRANSCRIPT].columnIndex("STRUCTURE");
    sindex gene_name_idx, trans_name_idx;
    String tbl_name, gene_id, gene_name, gene_type, gene_description;
    while (gff.next()) {
        auto &data = gff.data();
        if (hs_gff[data.source] && hs_gff[data.source][data.type]) {
            SDictionary row;
            tbl_name = hs_gff[data.source][data.type];
            if (!chridx.hasKey(data.seqid)) continue;
            row["CHROMOSOME"] = chridx[data.seqid];
            row["START"] = data.begin;
            row["END"] = data.end;
            if (tbl_name == "GENE") {
                row["STRAND"] = data.strand;
                if (data.attribute.hasKey("gene_id")) gene_id = data.attribute["gene_id"];
                else if (data.attribute.hasKey("ID"))
                    gene_id = data.attribute["ID"].substring(data.attribute["ID"].find(":")+1);
                else {
                    gene_id = "";
					std::cout << "Gene ID unknown." << std::endl;
                    continue;
                }
                gene_name_idx[gene_id] = table[GENE].rowCount();
                row["GENE_ID"] = gene_id;
                
                if (data.attribute.hasKey("Name")) gene_name = data.attribute["Name"];
                else gene_name = gene_id;
                row["NAME"] = gene_name;
                
                int type = 0;
                if (data.attribute.hasKey("biotype")) gene_type = data.attribute["biotype"];
                else gene_type = "";
                if(data.type == "gene") {
                    if (gene_type.contain("protein_coding") ||
                        gene_type.contain("TEC") ||
                        gene_type.contain("TR_") ||
                        gene_type.contain("IG_")) type = sbio::PROTEN_CODING;
                    else if (gene_type.contain("lncRNA")) type = LNC_RNA;
                    else if (gene_type.contain("ncRNA")) type = NC_RNA;
                    else if (gene_type.contain("pseudo")) type = sbio::PSEUDOGENE;
                }
                else if (data.type == "ncRNA_gene") {
                    if (gene_type.contain("tRNA")) type = sbio::T_RNA;
                    else if (gene_type.contain("rRNA")) type = sbio::R_RNA;
                    else if (gene_type.contain("sRNA")) type = sbio::SMALL_RNA;
                    else if (gene_type.contain("snRNA")) type = sbio::SN_RNA;
                    else if (gene_type.contain("snoRNA")) type = sbio::SNO_RNA;
                    else if (gene_type.contain("scRNA")) type = sbio::SC_RNA;
                    else if (gene_type.contain("scaRNA")) type = sbio::SCA_RNA;
                    else if (gene_type.contain("miRNA")) type = sbio::MI_RNA;
                    else if (gene_type.contain("lncRNA")) type = sbio::LNC_RNA;
                    else if (gene_type.contain("lincRNA")) type = sbio::LINC_RNA;
                    else if (gene_type.contain("antisense")) type = sbio::AS_RNA;
                    else if (gene_type.contain("ribozyme")) type = sbio::RIBOZYME;
                    else type = sbio::NON_CODING;
                }
                else if (data.type == "bidirectional_promoter_lncRNA") type = sbio::LNC_RNA;
                else if (data.type == "pseudogene") type = sbio::PSEUDOGENE;
                row["TYPE"] = type;
                if (data.attribute.hasKey("description")) gene_description = data.attribute["description"];
                else gene_description = "";
                SCode::urlDecode(gene_description);
                row["DESCRIPTION"] = gene_description;
                table[GENE].addRow(row);
            }
            else if (tbl_name == "TRANSCRIPT") {
                int gr = -1;
                gene_id = data.attribute["Parent"].substring(data.attribute["Parent"].find(":")+1);
                gr = gene_name_idx[gene_id];
                row["GENE_ID"] = gr+1;
                row["NAME"] = data.attribute["Name"];
                
                if (data.type=="mRNA") row["TYPE"] = sbio::M_RNA;
                else if (data.type=="tRNA") row["TYPE"] = sbio::T_RNA;
                else if (data.type=="rRNA") row["TYPE"] = sbio::R_RNA;
                else if (data.type=="ncRNA") row["TYPE"] = sbio::NC_RNA;
                else if (data.type=="lnc_RNA") row["TYPE"] = sbio::LNC_RNA;
                else if (data.type=="scRNA") row["TYPE"] = sbio::SC_RNA;
                else if (data.type=="snRNA") row["TYPE"] = sbio::SN_RNA;
                else if (data.type=="snoRNA") row["TYPE"] = sbio::SNO_RNA;
                else if (data.type=="miRNA") row["TYPE"] = sbio::MI_RNA;
                else if (data.type=="pseudogenic_transcript") row["TYPE"] = sbio::NC_RNA;
                else if (data.type=="transcript") row["TYPE"] = 0;
                else if (data.type=="unconfirmed_transcript") row["TYPE"] = 0;
                else if (data.type=="C_gene_segment") row["TYPE"] = sbio::M_RNA;
                else if (data.type=="D_gene_segment") row["TYPE"] = sbio::M_RNA;
                else if (data.type=="J_gene_segment") row["TYPE"] = sbio::M_RNA;
                else if (data.type=="V_gene_segment") row["TYPE"] = sbio::M_RNA;
                else if (data.type=="three_prime_overlapping_ncrna") row["TYPE"] = sbio::NC_RNA;
                else if (data.type=="vaultRNA_primary_transcript") row["TYPE"] = sbio::VT_RNA;
                
                trans_name_idx[data.attribute["transcript_id"]] = table[TRANSCRIPT].rowCount();
                table[TRANSCRIPT].addRow(row);
                table[GENE][gr][trs_col_index_g].add(table[TRANSCRIPT].rowCount());
            }
            else if (tbl_name == "STRUCTURE") {
                int tr = -1;
                auto trans_id = data.attribute["Parent"].substring(data.attribute["Parent"].find(":")+1);
                tr = trans_name_idx[trans_id];
                
                if (data.type=="CDS") row["TYPE"] = sbio::CDS;
                else if (data.type=="five_prime_UTR") row["TYPE"] = sbio::UTR5;
                else if (data.type=="three_prime_UTR") row["TYPE"] = sbio::UTR3;
                else if (data.type=="exon") row["TYPE"] = sbio::EXON;
                
                row["TRANSCRIPT_ID"] = tr+1;
                table[STRUCTURE].addRow(row);
                table[TRANSCRIPT][tr][strct_col_index_t].add(table[STRUCTURE].rowCount());
            }
        }
    }
}
extern "C" {
	splugin makeDB(STable* table, SDictionary& dict, SBSeqList* ref) {
		loadGFF_HS(table, dict["gff"], ref->index);
		return 0;
	}
}