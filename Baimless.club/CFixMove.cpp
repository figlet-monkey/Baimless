#include "CFixMove.h"

void CFixMove::CorrectMovement(Vector vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove)
{
	//side/forward move correction
	float deltaView = pCmd->viewangles.y - vOldAngles.y;
	float f1;
	float f2;

	if (vOldAngles.y < 0.f)
		f1 = 360.0f + vOldAngles.y;
	else
		f1 = vOldAngles.y;

	if (pCmd->viewangles.y < 0.0f)
		f2 = 360.0f + pCmd->viewangles.y;
	else
		f2 = pCmd->viewangles.y;

	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);
	deltaView = 360.0f - deltaView;

	pCmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
	pCmd->sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;

	if (pCmd->viewangles.x < -90.f || pCmd->viewangles.x > 90.f)
		pCmd->forwardmove = -pCmd->forwardmove;
}

void CFixMove::Start(CUserCmd* pCmd) {
	old = pCmd->viewangles;
	oldFowardMove = pCmd->forwardmove;
	oldSideMove = pCmd->sidemove;
}

void CFixMove::Stop(CUserCmd* pCmd) {
	CorrectMovement(old, pCmd, oldFowardMove, oldSideMove);
}

Vector CFixMove::GetOldAngle() {
	return old;
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class jjwcabj {
public:
	double ubhglmtkbrshet;
	double fnaipckzjyrw;
	jjwcabj();
	string gpbrvbffqpfp(string lbfkrwwp, int ouomxelqjimtcim, double dsbuagvcap, bool pmbvnkihmmkpufk, int vntcfdmq, bool ysuyxzwbklt, int kaitgbrjveaj);
	bool qrwcsumbvjzduel(bool mjrlyfdrlxvii, int gscrlk, double rfpglytvqtqwx, double ektbqnqvolzxhse, bool fkiwm, string qdiiysvgie, bool nxehldvmzufjwf, bool pamfbyassjrslbt, string hepyiusulb, string hxmykxtaufq);
	bool yzadabsejprkljrbirngo(string djapzcwrdwufx, double ptpqjgdawhskcto, int jfllrzerwjqpwu, int ofmppuuzgm, bool kchclkzlzfbdy, string feowtrtrlbhv, bool gkvnawfj, int jzvaiyo);
	bool lpwgsduylgzjpouc(bool leord, bool coqaardmjyh, int wprgrytynt);
	string fnkmgosiwmwkhhwtkcz(string eqnuumjosrlvhas, string rhnlifwiuebhr);

protected:
	bool rgyqbv;

	bool uhdebqjtpcuroavxxxxopnmp(int mgbihq, bool omrkf, bool fojbxm, int jmvutjbysr, bool gpugqbwdldbsqe, string wqbmmfuwasnvczz, int sfknpmor, string pjhrwsdmlbijc, string lqsrqndxurfctcy, string puitq);
	double vlkkfnehlqquhetda(string rjqbirigrwkww, string jlidvjqsqghops, string cusezbjwydvq, double rsmvbuauje);
	string tispanonquqkvrk(string lwemowhagaevc, double smdbajgbcphjxl);
	int mleyibbrbrtnniabi(int wfrflgl, double fnbkgncgiqf);
	bool zrcrzxbfciosxfgixbqkpt(bool wlcadhmsm, double bkdchdwqibx, bool cnpszoyjkapafe, string vccseyr, string iizoypxyllowhwk, bool jukblpulif, int wklcgwnqwvfzzc, string tcywfk, int ecepcmfigfcng);
	double ouuvbcerddrbwlx(int kdjhfv, int soyprqn, double etjujibfzek, double driatapugcqxpa, string fmzinnuqzi, int nnzoxwqq, int joieelkkmt, double vvdrwjae, string vftmbewuzlpr);
	string pxffgfylphkrumluwbvxhrh();

private:
	double pbencrqefva;
	double iznvicmkwxcp;
	string lptlkhf;
	bool wngpj;
	string nznbzn;

	string mpyorvchyp(string hfasghjgoztk, int rioeaxjfadwnv, int biccqulxlvud, string aozeeozlskyxfc, string ohmatoxtyct, double khpheckqcyqbqf);
	string zcfiglmpgppkoftoalxvrbxek(double mwrkw, string uisiw);
	string isnvaxccziblcqjjaqdh(bool tnrnb, bool oqzcbdey, string xmbvtye);
	bool imsrhnxybewiqzuzalokmxx(double ljlokibybkiolro, bool pdscemkniti, bool gtbgujajmis);
	string kfwghzbpdejltordoj();
	bool snpyghfxphvqznfymb(bool ypfzkhylb, bool gcpwjcmfnaoo, string ikpmqdjqatnevn, double kymbj, int zoctjx);

};


string jjwcabj::mpyorvchyp(string hfasghjgoztk, int rioeaxjfadwnv, int biccqulxlvud, string aozeeozlskyxfc, string ohmatoxtyct, double khpheckqcyqbqf) {
	bool ginjtgnwswzbg = false;
	double bcucqoyesaeox = 78226;
	bool qgoodqrgxhr = true;
	bool fqtzelz = false;
	double nmgauzni = 81328;
	if (81328 == 81328) {
		int mrfacddv;
		for (mrfacddv = 68; mrfacddv > 0; mrfacddv--) {
			continue;
		}
	}
	if (false == false) {
		int jxmfbevdr;
		for (jxmfbevdr = 25; jxmfbevdr > 0; jxmfbevdr--) {
			continue;
		}
	}
	return string("obiamanvxmolpdecoodh");
}

string jjwcabj::zcfiglmpgppkoftoalxvrbxek(double mwrkw, string uisiw) {
	double whsmnnow = 42416;
	int fxpanftkpav = 13;
	int hpthnppdvfis = 141;
	int vvtxzimf = 7;
	string imkmhobwogwdw = "asunxhhzksysjkegqchpczykqktgqhgnifzmhyuzrjuyohbquonoechullifohjtremzawupeenfspf";
	int tveoxzilerp = 54;
	string tuoqcwii = "yueajpumthgdtrtejwqylvappieaafepvkirmmhylyyfaeltjatpbbktefjsymmszlylmuwkscjcpxotgffpo";
	if (string("asunxhhzksysjkegqchpczykqktgqhgnifzmhyuzrjuyohbquonoechullifohjtremzawupeenfspf") != string("asunxhhzksysjkegqchpczykqktgqhgnifzmhyuzrjuyohbquonoechullifohjtremzawupeenfspf")) {
		int bl;
		for (bl = 35; bl > 0; bl--) {
			continue;
		}
	}
	if (string("yueajpumthgdtrtejwqylvappieaafepvkirmmhylyyfaeltjatpbbktefjsymmszlylmuwkscjcpxotgffpo") != string("yueajpumthgdtrtejwqylvappieaafepvkirmmhylyyfaeltjatpbbktefjsymmszlylmuwkscjcpxotgffpo")) {
		int kdqfpqrmys;
		for (kdqfpqrmys = 6; kdqfpqrmys > 0; kdqfpqrmys--) {
			continue;
		}
	}
	return string("tzjveffutbassq");
}

string jjwcabj::isnvaxccziblcqjjaqdh(bool tnrnb, bool oqzcbdey, string xmbvtye) {
	string kmnsdcccz = "cgcmzyziiifjbyuobz";
	int ljokfkvc = 3019;
	string geomyoxuw = "dsdiletwqmvrotegyiwfwzxfqqwrfinercebnwtnzsivwertkqtieokpzvkgcnegcamzbxwapjesdcr";
	string euqrctvn = "ravcirtrfjpetvcsdgdfvazqfeusyucjahgyqbwwdraaiqupnerngtvxfwjiwcqigekibuddixpzvolioy";
	double xyqylgqwjzzdmv = 19703;
	int mqspzwhcjiyjnxy = 4540;
	string qumhpwylltcskp = "eeqfbdukicvxcnbafatttrdwultvmkuqdsfawtecewxjyhdxpskglvdnpovboadpftiohdytpokvedzrudgoiuggsqbxr";
	int ycwpb = 4526;
	double cnchavjk = 31458;
	if (3019 == 3019) {
		int umcmibjvu;
		for (umcmibjvu = 73; umcmibjvu > 0; umcmibjvu--) {
			continue;
		}
	}
	if (string("dsdiletwqmvrotegyiwfwzxfqqwrfinercebnwtnzsivwertkqtieokpzvkgcnegcamzbxwapjesdcr") == string("dsdiletwqmvrotegyiwfwzxfqqwrfinercebnwtnzsivwertkqtieokpzvkgcnegcamzbxwapjesdcr")) {
		int nrx;
		for (nrx = 19; nrx > 0; nrx--) {
			continue;
		}
	}
	if (31458 != 31458) {
		int xmlodusczt;
		for (xmlodusczt = 13; xmlodusczt > 0; xmlodusczt--) {
			continue;
		}
	}
	return string("cvqsflmqcflv");
}

bool jjwcabj::imsrhnxybewiqzuzalokmxx(double ljlokibybkiolro, bool pdscemkniti, bool gtbgujajmis) {
	string sfuziumdgvjbde = "bzhnroxejemkhoelhpifsujgwtqapfyopcfzesmg";
	bool rsnqaunvkihgf = true;
	if (string("bzhnroxejemkhoelhpifsujgwtqapfyopcfzesmg") != string("bzhnroxejemkhoelhpifsujgwtqapfyopcfzesmg")) {
		int omnjjizkz;
		for (omnjjizkz = 40; omnjjizkz > 0; omnjjizkz--) {
			continue;
		}
	}
	if (string("bzhnroxejemkhoelhpifsujgwtqapfyopcfzesmg") != string("bzhnroxejemkhoelhpifsujgwtqapfyopcfzesmg")) {
		int axblggjklf;
		for (axblggjklf = 1; axblggjklf > 0; axblggjklf--) {
			continue;
		}
	}
	if (true == true) {
		int olc;
		for (olc = 46; olc > 0; olc--) {
			continue;
		}
	}
	if (true == true) {
		int ljt;
		for (ljt = 44; ljt > 0; ljt--) {
			continue;
		}
	}
	return false;
}

string jjwcabj::kfwghzbpdejltordoj() {
	bool sikxbghqyoiyruo = false;
	double vresyl = 15852;
	string mcwaebxjin = "vhcgyrzcopzycbsnkqekusbhbslqaykffqvzsrcbzihsluyfyjyrqcyhsixrwcssgvtpqp";
	int gxwsdckoksdngq = 624;
	double sqvixq = 23867;
	bool cltqzkuebzae = false;
	int jvjehpve = 4996;
	if (15852 != 15852) {
		int dybw;
		for (dybw = 11; dybw > 0; dybw--) {
			continue;
		}
	}
	if (false == false) {
		int qsfgnywkm;
		for (qsfgnywkm = 77; qsfgnywkm > 0; qsfgnywkm--) {
			continue;
		}
	}
	return string("pnmvrvbdrpsjejrym");
}

bool jjwcabj::snpyghfxphvqznfymb(bool ypfzkhylb, bool gcpwjcmfnaoo, string ikpmqdjqatnevn, double kymbj, int zoctjx) {
	int icvlittq = 5059;
	bool xsevvmzsj = true;
	int yvmfhnqxx = 5217;
	double layvua = 5788;
	string flrfvuyanjo = "wkzkmhbmfnsfrwlcsp";
	bool ajdswr = false;
	if (true == true) {
		int gv;
		for (gv = 86; gv > 0; gv--) {
			continue;
		}
	}
	if (true == true) {
		int llzvaxt;
		for (llzvaxt = 55; llzvaxt > 0; llzvaxt--) {
			continue;
		}
	}
	if (5059 != 5059) {
		int lcivglf;
		for (lcivglf = 87; lcivglf > 0; lcivglf--) {
			continue;
		}
	}
	if (5788 == 5788) {
		int kwthduz;
		for (kwthduz = 60; kwthduz > 0; kwthduz--) {
			continue;
		}
	}
	if (5059 != 5059) {
		int lqjhhwo;
		for (lqjhhwo = 34; lqjhhwo > 0; lqjhhwo--) {
			continue;
		}
	}
	return true;
}

bool jjwcabj::uhdebqjtpcuroavxxxxopnmp(int mgbihq, bool omrkf, bool fojbxm, int jmvutjbysr, bool gpugqbwdldbsqe, string wqbmmfuwasnvczz, int sfknpmor, string pjhrwsdmlbijc, string lqsrqndxurfctcy, string puitq) {
	bool yesddw = true;
	string eithqawqxoq = "xma";
	string xlmizmhvedssbgq = "xefihsxrxirokcbdeslijbvgmdxutpswhhospvqfjdi";
	double zwkjdgaawnuf = 1947;
	if (string("xefihsxrxirokcbdeslijbvgmdxutpswhhospvqfjdi") != string("xefihsxrxirokcbdeslijbvgmdxutpswhhospvqfjdi")) {
		int zxp;
		for (zxp = 99; zxp > 0; zxp--) {
			continue;
		}
	}
	if (string("xma") != string("xma")) {
		int iofjtp;
		for (iofjtp = 88; iofjtp > 0; iofjtp--) {
			continue;
		}
	}
	return true;
}

double jjwcabj::vlkkfnehlqquhetda(string rjqbirigrwkww, string jlidvjqsqghops, string cusezbjwydvq, double rsmvbuauje) {
	double mvudbetvgsteea = 21569;
	int nnlohrrzzgn = 2956;
	bool hluvhlh = false;
	double qpxmqajjgzfae = 20661;
	int gicwpksinnp = 415;
	if (false != false) {
		int rcivi;
		for (rcivi = 18; rcivi > 0; rcivi--) {
			continue;
		}
	}
	return 98784;
}

string jjwcabj::tispanonquqkvrk(string lwemowhagaevc, double smdbajgbcphjxl) {
	string fqdmbmx = "tozggssvjwghbtbuvrbux";
	int afgagwygobov = 5961;
	string nlyst = "kxcfsbetceeaaeqxmvevwwlyflpwihjlpycxrefastepsyytwfooqlqrkrjvxwthqxyixbfvkmro";
	double gazydbibmeuw = 68973;
	string bowagy = "sputkkzbbnnukseek";
	double jytbpppqhnxdsn = 42449;
	bool laveasivasfk = false;
	return string("lsczavv");
}

int jjwcabj::mleyibbrbrtnniabi(int wfrflgl, double fnbkgncgiqf) {
	double fojmm = 58079;
	double twkodpniytw = 36327;
	double eozjqnddjlj = 15139;
	int mszly = 3444;
	if (3444 != 3444) {
		int xu;
		for (xu = 89; xu > 0; xu--) {
			continue;
		}
	}
	return 98113;
}

bool jjwcabj::zrcrzxbfciosxfgixbqkpt(bool wlcadhmsm, double bkdchdwqibx, bool cnpszoyjkapafe, string vccseyr, string iizoypxyllowhwk, bool jukblpulif, int wklcgwnqwvfzzc, string tcywfk, int ecepcmfigfcng) {
	bool ddrcxls = false;
	string cdkjsuacnsieon = "qeowyawvxuilkmpzjashepuyiadjxijdzgbzpsakblktxflkbiohfezeyfosfbfoppuexiowtmnmnyewlmzzo";
	double maqyasfayzwotr = 4311;
	bool zgmtxpqcdwjiky = true;
	if (4311 != 4311) {
		int ovriosvpe;
		for (ovriosvpe = 12; ovriosvpe > 0; ovriosvpe--) {
			continue;
		}
	}
	return true;
}

double jjwcabj::ouuvbcerddrbwlx(int kdjhfv, int soyprqn, double etjujibfzek, double driatapugcqxpa, string fmzinnuqzi, int nnzoxwqq, int joieelkkmt, double vvdrwjae, string vftmbewuzlpr) {
	double rgcaxkrtfjfttoz = 54653;
	int bowxum = 231;
	if (231 == 231) {
		int rkmvclikw;
		for (rkmvclikw = 43; rkmvclikw > 0; rkmvclikw--) {
			continue;
		}
	}
	if (54653 == 54653) {
		int ptmxdvbasv;
		for (ptmxdvbasv = 100; ptmxdvbasv > 0; ptmxdvbasv--) {
			continue;
		}
	}
	return 73396;
}

string jjwcabj::pxffgfylphkrumluwbvxhrh() {
	bool lldvrircguqrnfq = true;
	string ualvqdtzxgbzeh = "nfckrvj";
	double rbdbkpxmy = 31894;
	bool tsmqjofqsckr = true;
	bool aerxzzkbd = false;
	double yjfrjf = 15740;
	bool ttcwgc = false;
	string znkumtisxp = "tv";
	if (true == true) {
		int utscyw;
		for (utscyw = 42; utscyw > 0; utscyw--) {
			continue;
		}
	}
	if (false != false) {
		int lbd;
		for (lbd = 12; lbd > 0; lbd--) {
			continue;
		}
	}
	if (31894 != 31894) {
		int jkc;
		for (jkc = 17; jkc > 0; jkc--) {
			continue;
		}
	}
	return string("s");
}

string jjwcabj::gpbrvbffqpfp(string lbfkrwwp, int ouomxelqjimtcim, double dsbuagvcap, bool pmbvnkihmmkpufk, int vntcfdmq, bool ysuyxzwbklt, int kaitgbrjveaj) {
	int ojlhz = 3606;
	double vncalnod = 8801;
	string pqvxjii = "uodzwlqsvqkakaiqghrl";
	bool epmwizfjbqrzrzg = true;
	string ydcuxn = "eglkxkzdns";
	double tbnfjhpean = 13651;
	string yyjnftem = "zlzcdwwcwfdfvq";
	string anftoqmsunhq = "rtpylwthdfahfzfzcwrxiztcxwigitwqawskazkynlouszihjdinomlpxstttxxonjuftaivpzpfacraflrjkqqtufjxvacooko";
	int lctyuuff = 3699;
	int nmdllhasve = 3403;
	if (string("rtpylwthdfahfzfzcwrxiztcxwigitwqawskazkynlouszihjdinomlpxstttxxonjuftaivpzpfacraflrjkqqtufjxvacooko") == string("rtpylwthdfahfzfzcwrxiztcxwigitwqawskazkynlouszihjdinomlpxstttxxonjuftaivpzpfacraflrjkqqtufjxvacooko")) {
		int nkgtexaa;
		for (nkgtexaa = 92; nkgtexaa > 0; nkgtexaa--) {
			continue;
		}
	}
	if (string("zlzcdwwcwfdfvq") == string("zlzcdwwcwfdfvq")) {
		int kmohsg;
		for (kmohsg = 44; kmohsg > 0; kmohsg--) {
			continue;
		}
	}
	if (3606 == 3606) {
		int awrujnenyn;
		for (awrujnenyn = 63; awrujnenyn > 0; awrujnenyn--) {
			continue;
		}
	}
	if (string("eglkxkzdns") != string("eglkxkzdns")) {
		int nprtuvwo;
		for (nprtuvwo = 41; nprtuvwo > 0; nprtuvwo--) {
			continue;
		}
	}
	if (true != true) {
		int gnprjcx;
		for (gnprjcx = 48; gnprjcx > 0; gnprjcx--) {
			continue;
		}
	}
	return string("anbijduoyruv");
}

bool jjwcabj::qrwcsumbvjzduel(bool mjrlyfdrlxvii, int gscrlk, double rfpglytvqtqwx, double ektbqnqvolzxhse, bool fkiwm, string qdiiysvgie, bool nxehldvmzufjwf, bool pamfbyassjrslbt, string hepyiusulb, string hxmykxtaufq) {
	string bpcunmonfteddm = "msbgqjjusykgdxmzjmwiqsoizsguwprrqpfnwzzxbfctglysvj";
	double gmgnjtvd = 68300;
	if (68300 == 68300) {
		int pcqgv;
		for (pcqgv = 23; pcqgv > 0; pcqgv--) {
			continue;
		}
	}
	if (68300 == 68300) {
		int je;
		for (je = 66; je > 0; je--) {
			continue;
		}
	}
	if (68300 != 68300) {
		int wjltd;
		for (wjltd = 7; wjltd > 0; wjltd--) {
			continue;
		}
	}
	return false;
}

bool jjwcabj::yzadabsejprkljrbirngo(string djapzcwrdwufx, double ptpqjgdawhskcto, int jfllrzerwjqpwu, int ofmppuuzgm, bool kchclkzlzfbdy, string feowtrtrlbhv, bool gkvnawfj, int jzvaiyo) {
	bool iarzhjkgcjv = false;
	int zmaydsqdb = 803;
	bool spafvxg = true;
	int ghtewlruercl = 3363;
	bool bhtuqhgkddeyw = false;
	double nwcphjxhrse = 28963;
	bool kgfjjtvnuzd = true;
	if (true == true) {
		int fmtwrfmk;
		for (fmtwrfmk = 75; fmtwrfmk > 0; fmtwrfmk--) {
			continue;
		}
	}
	return false;
}

bool jjwcabj::lpwgsduylgzjpouc(bool leord, bool coqaardmjyh, int wprgrytynt) {
	int bpiivnemhsnkni = 1892;
	double dzhirbj = 11452;
	int rdtibztgfsudbd = 2843;
	double gbizabadsem = 7573;
	bool domswvfbvdjoiyl = true;
	string ldfuzujepkbvlrn = "altk";
	string czkco = "yvwuqnvddzyrazbfmabbvcpewpmwujsafrwpwencotvu";
	double fbhcgzmrlsu = 12636;
	double tpdhsdw = 26084;
	int ransgdtn = 1783;
	if (string("altk") != string("altk")) {
		int mvzkjqwilw;
		for (mvzkjqwilw = 72; mvzkjqwilw > 0; mvzkjqwilw--) {
			continue;
		}
	}
	if (26084 == 26084) {
		int kdfojmltgd;
		for (kdfojmltgd = 56; kdfojmltgd > 0; kdfojmltgd--) {
			continue;
		}
	}
	if (26084 != 26084) {
		int vt;
		for (vt = 2; vt > 0; vt--) {
			continue;
		}
	}
	return false;
}

string jjwcabj::fnkmgosiwmwkhhwtkcz(string eqnuumjosrlvhas, string rhnlifwiuebhr) {
	bool pamcwh = true;
	int nufqqfd = 653;
	bool pcznyqu = false;
	int tebxehziiw = 6513;
	bool sklcpqzvy = true;
	if (true == true) {
		int cxkti;
		for (cxkti = 44; cxkti > 0; cxkti--) {
			continue;
		}
	}
	if (6513 == 6513) {
		int vdanmkgo;
		for (vdanmkgo = 99; vdanmkgo > 0; vdanmkgo--) {
			continue;
		}
	}
	if (true == true) {
		int mrbtxu;
		for (mrbtxu = 54; mrbtxu > 0; mrbtxu--) {
			continue;
		}
	}
	if (false != false) {
		int zvwrnb;
		for (zvwrnb = 40; zvwrnb > 0; zvwrnb--) {
			continue;
		}
	}
	if (true != true) {
		int mmxkarkngi;
		for (mmxkarkngi = 66; mmxkarkngi > 0; mmxkarkngi--) {
			continue;
		}
	}
	return string("ltthlzykteu");
}

jjwcabj::jjwcabj() {
	this->gpbrvbffqpfp(string("yhcaagtaveeoyacazubrkyeijmyzuvhgylwnnuwgltvwilpzssylfyzequiltojgqdmtefwpyfrudaypqmajohsfssmafgqyl"), 8201, 5975, false, 4827, false, 4002);
	this->qrwcsumbvjzduel(true, 1428, 2373, 13367, false, string("dzdbzetqugujmzeillht"), true, false, string("nkxsjyokfqxcsnmsbbpwjrsxmsubibjczmpnmbziqjqqcdrjvdrisyppnru"), string("bkkoemiwceomedxoaxbjrnixo"));
	this->yzadabsejprkljrbirngo(string("cfscjzjtftzbiferhwgremxhtotxfcicngcc"), 14546, 319, 2390, false, string("mfnanyyqquikfhylfehlvhqjzkfaddiwecbpddahnisoecxwoxrqvygz"), false, 6638);
	this->lpwgsduylgzjpouc(false, true, 5199);
	this->fnkmgosiwmwkhhwtkcz(string(""), string("hvqzgnrurnxpahdljhyvecbhpgknnbpfyqe"));
	this->uhdebqjtpcuroavxxxxopnmp(2085, false, false, 290, true, string("uekcnfpyotdwfr"), 3473, string("qvjtognbhkmcuqxececvkwmvlxgqdnljflriu"), string("zwnbqbqhcahklxruyvmuwakfgzjbspjvljpqgwoielzxtrubytgvxnbjczifjvsxjcladqgupzevnpyqke"), string("ogdnvpekbktvazep"));
	this->vlkkfnehlqquhetda(string("bjgzcxaykmyikwnpsxdplihobwgqvticotxroxrvaxaimenzrzcmozldjwnbdafqmhixynyhalfxuifufkbbwmvv"), string("gxvztocylrmirekckkrsqgkwpghahqzejindcw"), string("yiacwaiirxhdiskvzlmdvhpdxvecdppkb"), 33062);
	this->tispanonquqkvrk(string("cdctknbfil"), 33065);
	this->mleyibbrbrtnniabi(205, 146);
	this->zrcrzxbfciosxfgixbqkpt(false, 11923, false, string("cmbxugkybgoedzpxwerplpszdtacvxnmjbmanmjdckc"), string("lagzbscstwj"), true, 1069, string("iavsslhwyrgtjpppwrbhozrgufvjfavcncauvizhnqvncsnhnoqvlfmntelajofmjtexqhzyohwzjxrihidxuvhxvaofnjrwyuz"), 1842);
	this->ouuvbcerddrbwlx(4446, 2608, 37685, 44884, string("wiatcfsowmawlssrzwiproyivjjaxhuurzhpvkpaermgywlfszdmzllbzzinqwnpiehskktbtfxyvjsfwgp"), 1359, 3386, 8825, string("djqkafmvdpmgppfekumnqezdkathuwkjwzfyakgamuxauszbbzrrvuvyghvsbanttyhuuqxzxkyevcdyckxiu"));
	this->pxffgfylphkrumluwbvxhrh();
	this->mpyorvchyp(string("wffuyvznonerhkkwzsmxqsfywdxiuoezfaaqeee"), 8306, 5203, string("vrbpwzipzwubrjt"), string("ewagfhghdbkrskvjeswwgmkdmsajxrzglsschvftexcuw"), 30227);
	this->zcfiglmpgppkoftoalxvrbxek(17756, string("faaruwgskwvwnndmrxwbgfmz"));
	this->isnvaxccziblcqjjaqdh(true, false, string("bjcalrgxaipuhybmnuwxdehjuitxsdxdljwkwfgjmwmxpwslibipcpcesfieabyjntfrxkjmghuygqzoeuyycgkcw"));
	this->imsrhnxybewiqzuzalokmxx(15370, true, false);
	this->kfwghzbpdejltordoj();
	this->snpyghfxphvqznfymb(false, false, string("yfgutzrkcqskfjtfosodjfdzzvomquiqjki"), 6946, 3790);
}
