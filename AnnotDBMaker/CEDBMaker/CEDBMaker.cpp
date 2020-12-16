#include "DBMaker.h"
#include "sapp/scuiapp.h"

using namespace slib;
using namespace slib::sbio;

SDictionary ce_gff = {
    kv("Genomic_canonical", V({ kv("assembly_component", "CONTIG") })),
    
    kv("absolute_pmap_position", V({ kv("gene", "GENE") })),
    kv("interpolated_pmap_position", V({ kv("gene", "GENE") })),
    kv("WormBase", V({
        kv("gene", "GENE"),
        
        kv("mRNA", "TRANSCRIPT"),
        kv("tRNA", "TRANSCRIPT"),
        kv("rRNA", "TRANSCRIPT"),
        kv("ncRNA", "TRANSCRIPT"),
        kv("nc_primary_transcript", "TRANSCRIPT"),
        kv("lincRNA", "TRANSCRIPT"),
        kv("miRNA", "TRANSCRIPT"),
        kv("antisense_RNA", "TRANSCRIPT"),
        kv("miRNA_primary_transcript", "TRANSCRIPT"),
        kv("pre_miRNA", "TRANSCRIPT"),
        kv("piRNA", "TRANSCRIPT"),
        kv("snRNA", "TRANSCRIPT"),
        kv("snoRNA", "TRANSCRIPT"),
        kv("scRNA", "TRANSCRIPT"),
        kv("pseudogenic_transcript", "TRANSCRIPT"),
        kv("pseudogenic_tRNA", "TRANSCRIPT"),
        kv("pseudogenic_rRNA", "TRANSCRIPT"),
        
        kv("CDS", "STRUCTURE"),
        kv("five_prime_UTR", "STRUCTURE"),
        kv("three_prime_UTR", "STRUCTURE"),
        kv("exon", "STRUCTURE"),
        kv("intron", "STRUCTURE")
    })),
    kv("WormBase_transposon", V({ kv("gene", "GENE") })),
    
    kv("Allele", V({
        kv("point_mutation", "MUTATION"),
        kv("substitution", "MUTATION"),
        kv("insertion_site", "MUTATION"),
        kv("sequence_alteration", "MUTATION"),
        kv("transposable_element_insertion_site", "MUTATION"),
        kv("deletion", "MUTATION"),
        kv("complex_substitution", "MUTATION")
    })),
    kv("CGH_allele", V({ kv("deletion", "MUTATION") })),
    kv("KO_consortium", V({
        kv("point_mutation", "MUTATION"),
        kv("insertion_site", "MUTATION"),
        kv("deletion", "MUTATION"),
        kv("complex_substitution", "MUTATION")
    })),
    kv("Mos_insertion_allele", V({ kv("transposable_element_insertion_site", "MUTATION") })),
    kv("NBP_knockout", V({
        kv("deletion", "MUTATION"),
        kv("complex_substitution", "MUTATION")
    })),
    kv("Variation_project", V({
        kv("point_mutation", "MUTATION"),
        kv("insertion_site", "MUTATION"),
        kv("deletion", "MUTATION")
    })),
    
    kv("CGH_allele_Polymorphism", V({ kv("deletion", "VARIATION") })),
    kv("Variation_project_Polymorphism", V({
        kv("SNP", "VARIATION"),
        kv("substitution", "VARIATION"),
        kv("insertion_site", "VARIATION"),
        kv("deletion", "VARIATION"),
        kv("complex_substitution", "VARIATION"),
        kv("tandem_duplication", "VARIATION")
    })),
    kv("Polymorphism", V({
        kv("insertion_site", "VARIATION"),
        kv("deletion", "VARIATION")
    })),
    kv("Million_mutation", V({
        kv("point_mutation", "VARIATION"),
        kv("insertion_site", "VARIATION"),
        kv("deletion", "VARIATION"),
        kv("complex_substitution", "VARIATION"),
        kv("tandem_duplication", "VARIATION")
    })),
    
    kv("Balanced_by_balancer", V({ kv("biological_region", "FEATURE") })),
    kv("binding_site", V({ kv("binding_site", "FEATURE") })),
    kv("binding_site_region", V({ kv("binding_site", "FEATURE") })),
    kv("enhancer", V({ kv("enhancer", "FEATURE") })),
    kv("histone_binding_site_region", V({ kv("histone_binding_site", "FEATURE") })),
    kv("operon", V({ kv("operon", "FEATURE") })),
    kv("promoter", V({ kv("promoter", "FEATURE") })),
    kv("regulatory_region", V({ kv("regulatory_region", "FEATURE") })),
    kv("SL1", V({ kv("SL1_acceptor_site", "FEATURE") })),
    kv("SL2", V({ kv("SL2_acceptor_site", "FEATURE") })),
    kv("TF_binding_site", V({ kv("TF_binding_site", "FEATURE") })),
    kv("TF_binding_site_region", V({ kv("TF_binding_site", "FEATURE") })),
    kv("TSS_region", V({ kv("TSS_region", "FEATURE") }))
};

void loadGFF_CE(STable *table, sindex gene_name_idx, const char *path, const sindex &chridx) {
    sbio::SGFFFile gff;
    gff.open(path);
    String tbl_name;
    sindex trans_name_idx;
    SDictionary row; row.reserve(32);
    size_t name_col_index_g = table[GENE].columnIndex("NAME"),
    trs_col_index_g = table[GENE].columnIndex("TRANSCRIPT"),
    attr_col_index_g = table[GENE].columnIndex("ATTRIBUTE"),
    strct_col_index_t = table[TRANSCRIPT].columnIndex("STRUCTURE");
    while (gff.next()) {
        row.clear();
        auto &data = gff.data();
        if (ce_gff[data.source] && ce_gff[data.source][data.type]) {
            tbl_name = ce_gff[data.source][data.type];
            row["CHROMOSOME"] = chridx[data.seqid];
            row["START"] = data.begin;
            row["END"] = data.end;
            if (tbl_name == "CONTIG") {
                row["NAME"] = data.attribute["Name"];
				table[CONTIG].addRow();
				table[CONTIG].updateRow(table[CONTIG].rowCount() - 1, row);
            }
            else if (tbl_name == "GENE") {
                auto gene_name = data.attribute["ID"].substring(data.attribute["ID"].find(":")+1);
                if (data.source == "WormBase") {
                    row["STRAND"] = data.strand;
                    if (gene_name_idx.hasKey(gene_name)) {
                        auto rowidx = gene_name_idx[gene_name]-1;
                        String type_str = data.attribute["biotype"];
                        if (type_str.beginWith("protein")) row["TYPE"] = sbio::PROTEIN_CODING;
                        else if (type_str.beginWith("nc")) row["TYPE"] = sbio::NON_CODING;
                        else if (type_str.beginWith("pseudo")) row["TYPE"] = sbio::PSEUDOGENE;
                        else if (type_str.beginWith("tRNA")) row["TYPE"] = sbio::T_RNA;
                        else if (type_str.beginWith("rRNA")) row["TYPE"] = sbio::R_RNA;
                        else if (type_str.beginWith("linc")) row["TYPE"] = sbio::LINC_RNA;
                        else if (type_str.beginWith("asRNA")) row["TYPE"] = sbio::AS_RNA;
                        else if (type_str.beginWith("miRNA")) row["TYPE"] = sbio::MI_RNA;
                        else if (type_str.beginWith("piRNA")) row["TYPE"] = sbio::PI_RNA;
                        else if (type_str.beginWith("snRNA")) row["TYPE"] = sbio::SN_RNA;
                        else if (type_str.beginWith("snoRNA")) row["TYPE"] = sbio::SNO_RNA;
                        else if (type_str.beginWith("scRNA")) row["TYPE"] = sbio::SC_RNA;
                        
                        String map = "";
                        if (data.attribute["interpolated_map_position"])
                            map = data.attribute["interpolated_map_position"];
                        else if (data.attribute["Note"]) map = data.attribute["Note"];
                        if (map.contain("cM")) row["ATTRIBUTE"] = "GMAP="+map.substring(0, map.find("cM")).transformed(sstyle::TRIMMING)+";";
                        else if (map.isNumeric()) row["ATTRIBUTE"] = "GMAP="+map.transformed(sstyle::TRIMMING)+";";
                        table[GENE].updateRow(rowidx, row);
                    }
                }
                else {
                    auto rowidx = table[GENE].find(gene_name, smath::VERTICAL, srange(0, name_col_index_g)).begin;
                    if (rowidx != NOT_FOUND) {
                        auto map = data.attribute["Note"];
                        if (map.contain("cM")) {
                            String mapval = "GMAP="+map.substring(0, map.find("cM")).transformed(sstyle::TRIMMING)+";";
                            table[GENE][rowidx][attr_col_index_g] = mapval;
                        }
                    }
                }
            }
            else if (tbl_name == "TRANSCRIPT") {
                auto gen_name = data.attribute["Parent"].substring(data.attribute["Parent"].find(":")+1);
                if (data.type=="mRNA") row["TYPE"] = sbio::M_RNA;
                else if (data.type=="tRNA") row["TYPE"] = sbio::T_RNA;
                else if (data.type=="rRNA") row["TYPE"] = sbio::R_RNA;
                else if (data.type=="tRNA") row["TYPE"] = sbio::T_RNA;
                else if (data.type.beginWith("nc")) row["TYPE"] = sbio::NC_RNA;
                else if (data.type=="lincRNA") row["TYPE"] = sbio::LINC_RNA;
                else if (data.type.beginWith("antisense")) row["TYPE"] = sbio::AS_RNA;
                else if (data.type.contain("miRNA")) row["TYPE"] = sbio::MI_RNA;
                else if (data.type=="piRNA") row["TYPE"] = sbio::PI_RNA;
                else if (data.type=="snRNA") row["TYPE"] = sbio::SN_RNA;
                else if (data.type=="snoRNA") row["TYPE"] = sbio::SNO_RNA;
                else if (data.type=="scRNA") row["TYPE"] = sbio::SC_RNA;
                else if (data.type.beginWith("pseudo")) row["TYPE"] = sbio::NC_RNA;
                row["NAME"] = data.attribute["Name"];
                sint gene_id = gene_name_idx[gen_name];
                row["GENE_ID"] = gene_id;
				table[TRANSCRIPT].addRow();
				table[TRANSCRIPT].updateRow(table[TRANSCRIPT].rowCount() - 1, row);
                trans_name_idx.set(row["NAME"], table[TRANSCRIPT].rowCount());
                table[GENE][gene_id-1][trs_col_index_g].add(table[TRANSCRIPT].rowCount());
            }
            else if (tbl_name == "STRUCTURE") {
                auto trans_names = data.attribute["Parent"].split(",");
                if (data.type=="CDS") row["TYPE"] = sbio::CDS;
                else if (data.type=="five_prime_UTR") row["TYPE"] = sbio::UTR5;
                else if (data.type=="three_prime_UTR") row["TYPE"] = sbio::UTR3;
                else if (data.type=="exon") row["TYPE"] = sbio::EXON;
                else if (data.type=="intron") row["TYPE"] = sbio::INTRON;
                else if (data.type=="miRNA") row["TYPE"] = sbio::PROCESSED;
                SArray parents(trans_names.size());
                sforin(t, 0, trans_names.size()) {
                    auto trans_name = trans_names[t].substring(trans_names[t].find(":")+1);
                    sint trans_id = trans_name_idx[trans_name];
                    parents[t] = trans_id;
                    table[TRANSCRIPT][trans_id-1][strct_col_index_t].add(table[STRUCTURE].rowCount()+1);
                }
                row["TRANSCRIPT_ID"] = parents;
				table[STRUCTURE].addRow();
				table[STRUCTURE].updateRow(table[STRUCTURE].rowCount() - 1, row);
            }
            else if (tbl_name == "MUTATION" || tbl_name == "VARIATION") {
                if (data.type=="SNP" || data.type=="point_mutation") row["TYPE"] = sbio::SNV;
                else if (data.type=="substitution")
                    row["TYPE"] = (row["END"]==row["START"]?sbio::SNV:sbio::MNV);
                else if (data.type=="insertion_site") row["TYPE"] = sbio::INSERTION;
                else if (data.type=="transposable_element_insertion_site") row["TYPE"] = sbio::INSERTION;
                else if (data.type=="deletion") row["TYPE"] = sbio::DELETION;
                else if (data.type=="complex_substitution") row["TYPE"] = sbio::DELETION|sbio::INSERTION;
                else if (data.type=="tandem_duplication") row["TYPE"] = sbio::DUPLICATION;
                else if (data.type=="sequence_alteration") row["TYPE"] = sbio::DELETION|sbio::INSERTION;
                if (tbl_name == "MUTATION") row["MUT_ID"] = data.attribute["variation"];
                else row["VAR_ID"] = data.attribute["variation"];
                row["NAME"] = data.attribute["public_name"];
                if (data.attribute["strain"]) row["STRAIN"] = data.attribute["strain"];
                if (data.attribute["substitution"]) {
                    String sub = data.attribute["substitution"];
                    row["ATTRIBUTE"] = "alt="+sub.substring(sub.find("/")+1)+";";
                }
                if (data.attribute["consequence"]) {
                    String conseq = data.attribute["consequence"];
                    int considx;
                    if (conseq == "Silent") considx = sbio::SILENT_MUT;
                    else if (conseq == "Nonsense") considx = sbio::NONSENSE;
                    else if (conseq == "Missense") considx = sbio::MISSENSE;
                    else if (conseq == "Readthrough") considx = sbio::MISSENSE;
                    else if (conseq == "Frameshift") considx = sbio::FRAME_SHIFT;
                    else if (conseq == "Splice_site") considx = sbio::SPLICE_SITE|SUBSTITUTION;
                    else if (conseq == "Intron") considx = sbio::INTRON|SUBSTITUTION;
                    else if (conseq == "Coding_exon") considx = sbio::EXON|SUBSTITUTION;
                    if (row["ATTRIBUTE"]) row["ATTRIBUTE"].string()<<"consequence="<<considx<<";";
                    else row["ATTRIBUTE"] = String("consequence=")<<considx<<";";
                }
				if (tbl_name == "MUTATION") {
					table[MUTANT].addRow();
					table[MUTANT].updateRow(table[MUTANT].rowCount() - 1, row);
				}
				else {
					table[VARIATION].addRow();
					table[VARIATION].updateRow(table[VARIATION].rowCount() - 1, row);
				}
            }
            else if (tbl_name == "FEATURE") {
                if (data.source.beginWith("Balanced")) {
                    if (data.attribute["balancer_type"]) {
                        auto type_str = data.attribute["balancer_type"];
                        int type = 0;
                        if (type_str == "Translocation") type = BALANCED_SITE;
                        else if (type_str == "Duplication") type = BALANCED_SITE;
                        else if (type_str == "Compound") type = BALANCED_SITE;
                        row["TYPE"] = type;
                    }
                    row["NAME"] = data.attribute["balancer"].substring(data.attribute["balancer"].find(":")+1);
                }
                if (data.source.beginWith("binding")) {
                    row["TYPE"] = BINDING_SITE;
                    if (data.attribute.hasKey("tf_name")) row["NAME"] = data.attribute["tf_name"];
                    else row["NAME"] = data.attribute["Name"];
                }
                if (data.source == "enhancer") {
                    row["TYPE"] = ENHANCER; row["NAME"] = data.attribute["Name"];
                }
                if (data.source.beginWith("histone")) {
                    row["TYPE"] = HISTONE_SITE; row["NAME"] = data.attribute["Name"];
                }
                if (data.source == "operon") {
                    row["TYPE"] = OPERON; row["NAME"] = data.attribute["Name"];
                }
                if (data.source == "promoter") {
                    row["TYPE"] = PROMOTER; row["NAME"] = data.attribute["Name"];
                }
                if (data.source.beginWith("regulatory")) {
                    row["TYPE"] = REGULATOR; row["NAME"] = data.attribute["Name"];
                }
                if (data.source == "SL1") {
                    row["TYPE"] = SPLICE_LEADER; row["NAME"] = "SL1";
                }
                if (data.source == "SL2") {
                    row["TYPE"] = SPLICE_LEADER; row["NAME"] = "SL2";
                }
                if (data.source.beginWith("TF_binding")) {
                    row["TYPE"] = TF_SITE; row["NAME"] = data.attribute["tf_name"];
                }
                if (data.source.beginWith("TSS")) {
                    row["TYPE"] = TSS_SITE; row["NAME"] = data.attribute["Name"];
                }
				table[FEATURE].addRow();
				table[FEATURE].updateRow(table[FEATURE].rowCount() - 1, row);
            }
        }
    }
}
extern "C" {
	splugin makeDB(STable* tables, SDictionary& dict, SBSeqList* ref) {
		sindex gene_name_index;
		sio::SFile file;
		String row;
		if (dict.hasKey("extra")) {
			auto extra = dict["extra"].parse(",", ":");
			if (extra.hasKey("gene")) {
				file.open(extra["gene"]);
				sint r = 1;
				tables[GENE].rows().reserve(1 << 17);
				while (!file.eof()) {
					file.readLine(row);
					stringarray array = row.split(",");
					if (array.size() < 5 || !(array[1].beginWith("WBG")) || array[4] == "Dead") continue;
					tables[GENE].addRow();
					tables[GENE].updateRow(tables[GENE].rowCount() - 1, {
						kv("ID", r),
						kv("GENE_ID", array[1]),
						kv("NAME", (array[2].empty() ? array[3] : array[2])),
						kv("OTHER_NAME",array[3])
						});
					gene_name_index[array[1]] = r;
					++r;
				}
				file.close();
			}
			if (extra.hasKey("description")) {
				file.open(extra["description"]);
				size_t col = tables[GENE].columnIndex("DESCRIPTION");
				if (col == NOT_FOUND) throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR);
				while (!file.eof()) {
					file.readLine(row);
					if (row.beginWith("#")) continue;
					auto dat = row.split(TAB);
					file.readTo(row, "=\n");
					auto descstr = row.substring(srange(row.find(":") + 2, row.find("\nProvisional description:") - 1));
					descstr.replace("\n", "");
					auto r = gene_name_index[dat[0]];
					if (r) tables[GENE].setValue(r - 1, col, descstr);
				}
				file.close();
			}
			if (extra.hasKey("balance")) {
				file.open(extra["balance"]);
				while (!file.eof()) {
					file.readLine(row);
					auto dat = row.split(TAB);
					tables[FEATURE].addRow();
					tables[FEATURE].updateRow(tables[FEATURE].rowCount() - 1, {
						kv("ID", snull),
						kv("TYPE", sbio::BALANCED_SITE),
						kv("NAME", dat[1]),
						kv("CHROMOSOME", dat[2].intValue()),
						kv("START", dat[3].intValue()),
						kv("END", dat[4].intValue())
						});
				}
				file.close();
			}
			//if (dict["tmallele"]) {}
		}
		if (dict["gff"]) loadGFF_CE(tables, gene_name_index, dict["gff"], ref->index);
		tables[CONTIG].addRow();
		tables[CONTIG].updateRow(tables[CONTIG].rowCount() - 1, {
			kv("ID", snull),
			kv("NAME", "MTCE"),
			kv("CHROMOSOME", ref->size() - 1),
			kv("START", 1),
			kv("END", ref->last()->length())
			});
		return 0;
	}
}


