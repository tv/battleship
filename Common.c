#include "Common.h"

void setLedOff(int led) {
	outr(PIOA_OER,  led);
	outr(PIOA_CODR, led);
}

void setLedOn(int led) {
	outr(PIOA_OER,  led);
	outr(PIOA_SODR, led);
}

void assignSinewave(void) {
	Sinewave = malloc( sizeof(short) * 1024 );

	Sinewave[0] = 0; Sinewave[1] = 201; Sinewave[2] = 402; Sinewave[3] = 603; Sinewave[4] = 804; Sinewave[5] = 1005; Sinewave[6] = 1206; Sinewave[7] = 1406; Sinewave[8] = 1607;
	Sinewave[9] = 1808; Sinewave[10] = 2009; Sinewave[11] = 2209; Sinewave[12] = 2410; Sinewave[13] = 2610; Sinewave[14] = 2811; Sinewave[15] = 3011; Sinewave[16] = 3211;
	Sinewave[17] = 3411; Sinewave[18] = 3611; Sinewave[19] = 3811; Sinewave[20] = 4011; Sinewave[21] = 4210; Sinewave[22] = 4409; Sinewave[23] = 4608; Sinewave[24] = 4807;
	Sinewave[25] = 5006; Sinewave[26] = 5205; Sinewave[27] = 5403; Sinewave[28] = 5601; Sinewave[29] = 5799; Sinewave[30] = 5997; Sinewave[31] = 6195; Sinewave[32] = 6392;
	Sinewave[33] = 6589; Sinewave[34] = 6786; Sinewave[35] = 6982; Sinewave[36] = 7179; Sinewave[37] = 7375; Sinewave[38] = 7571; Sinewave[39] = 7766; Sinewave[40] = 7961;
	Sinewave[41] = 8156; Sinewave[42] = 8351; Sinewave[43] = 8545; Sinewave[44] = 8739; Sinewave[45] = 8932; Sinewave[46] = 9126; Sinewave[47] = 9319; Sinewave[48] = 9511;
	Sinewave[49] = 9703; Sinewave[50] = 9895; Sinewave[51] = 10087; Sinewave[52] = 10278; Sinewave[53] = 10469; Sinewave[54] = 10659; Sinewave[55] = 10849; Sinewave[56] = 11038;
	Sinewave[57] = 11227; Sinewave[58] = 11416; Sinewave[59] = 11604; Sinewave[60] = 11792; Sinewave[61] = 11980; Sinewave[62] = 12166; Sinewave[63] = 12353; Sinewave[64] = 12539;
	Sinewave[65] = 12724; Sinewave[66] = 12909; Sinewave[67] = 13094; Sinewave[68] = 13278; Sinewave[69] = 13462; Sinewave[70] = 13645; Sinewave[71] = 13827; Sinewave[72] = 14009;
	Sinewave[73] = 14191; Sinewave[74] = 14372; Sinewave[75] = 14552; Sinewave[76] = 14732; Sinewave[77] = 14911; Sinewave[78] = 15090; Sinewave[79] = 15268; Sinewave[80] = 15446;
	Sinewave[81] = 15623; Sinewave[82] = 15799; Sinewave[83] = 15975; Sinewave[84] = 16150; Sinewave[85] = 16325; Sinewave[86] = 16499; Sinewave[87] = 16672; Sinewave[88] = 16845;
	Sinewave[89] = 17017; Sinewave[90] = 17189; Sinewave[91] = 17360; Sinewave[92] = 17530; Sinewave[93] = 17699; Sinewave[94] = 17868; Sinewave[95] = 18036; Sinewave[96] = 18204;
	Sinewave[97] = 18371; Sinewave[98] = 18537; Sinewave[99] = 18702; Sinewave[100] = 18867; Sinewave[101] = 19031; Sinewave[102] = 19194; Sinewave[103] = 19357; Sinewave[104] = 19519;
	Sinewave[105] = 19680; Sinewave[106] = 19840; Sinewave[107] = 20000; Sinewave[108] = 20159; Sinewave[109] = 20317; Sinewave[110] = 20474; Sinewave[111] = 20631; Sinewave[112] = 20787;
	Sinewave[113] = 20942; Sinewave[114] = 21096; Sinewave[115] = 21249; Sinewave[116] = 21402; Sinewave[117] = 21554; Sinewave[118] = 21705; Sinewave[119] = 21855; Sinewave[120] = 22004;
	Sinewave[121] = 22153; Sinewave[122] = 22301; Sinewave[123] = 22448; Sinewave[124] = 22594; Sinewave[125] = 22739; Sinewave[126] = 22883; Sinewave[127] = 23027; Sinewave[128] = 23169;
	Sinewave[129] = 23311; Sinewave[130] = 23452; Sinewave[131] = 23592; Sinewave[132] = 23731; Sinewave[133] = 23869; Sinewave[134] = 24006; Sinewave[135] = 24143; Sinewave[136] = 24278;
	Sinewave[137] = 24413; Sinewave[138] = 24546; Sinewave[139] = 24679; Sinewave[140] = 24811; Sinewave[141] = 24942; Sinewave[142] = 25072; Sinewave[143] = 25201; Sinewave[144] = 25329;
	Sinewave[145] = 25456; Sinewave[146] = 25582; Sinewave[147] = 25707; Sinewave[148] = 25831; Sinewave[149] = 25954; Sinewave[150] = 26077; Sinewave[151] = 26198; Sinewave[152] = 26318;
	Sinewave[153] = 26437; Sinewave[154] = 26556; Sinewave[155] = 26673; Sinewave[156] = 26789; Sinewave[157] = 26905; Sinewave[158] = 27019; Sinewave[159] = 27132; Sinewave[160] = 27244;
	Sinewave[161] = 27355; Sinewave[162] = 27466; Sinewave[163] = 27575; Sinewave[164] = 27683; Sinewave[165] = 27790; Sinewave[166] = 27896; Sinewave[167] = 28001; Sinewave[168] = 28105;
	Sinewave[169] = 28208; Sinewave[170] = 28309; Sinewave[171] = 28410; Sinewave[172] = 28510; Sinewave[173] = 28608; Sinewave[174] = 28706; Sinewave[175] = 28802; Sinewave[176] = 28897;
	Sinewave[177] = 28992; Sinewave[178] = 29085; Sinewave[179] = 29177; Sinewave[180] = 29268; Sinewave[181] = 29358; Sinewave[182] = 29446; Sinewave[183] = 29534; Sinewave[184] = 29621;
	Sinewave[185] = 29706; Sinewave[186] = 29790; Sinewave[187] = 29873; Sinewave[188] = 29955; Sinewave[189] = 30036; Sinewave[190] = 30116; Sinewave[191] = 30195; Sinewave[192] = 30272;
	Sinewave[193] = 30349; Sinewave[194] = 30424; Sinewave[195] = 30498; Sinewave[196] = 30571; Sinewave[197] = 30643; Sinewave[198] = 30713; Sinewave[199] = 30783; Sinewave[200] = 30851;
	Sinewave[201] = 30918; Sinewave[202] = 30984; Sinewave[203] = 31049; Sinewave[204] = 31113; Sinewave[205] = 31175; Sinewave[206] = 31236; Sinewave[207] = 31297; Sinewave[208] = 31356;
	Sinewave[209] = 31413; Sinewave[210] = 31470; Sinewave[211] = 31525; Sinewave[212] = 31580; Sinewave[213] = 31633; Sinewave[214] = 31684; Sinewave[215] = 31735; Sinewave[216] = 31785;
	Sinewave[217] = 31833; Sinewave[218] = 31880; Sinewave[219] = 31926; Sinewave[220] = 31970; Sinewave[221] = 32014; Sinewave[222] = 32056; Sinewave[223] = 32097; Sinewave[224] = 32137;
	Sinewave[225] = 32176; Sinewave[226] = 32213; Sinewave[227] = 32249; Sinewave[228] = 32284; Sinewave[229] = 32318; Sinewave[230] = 32350; Sinewave[231] = 32382; Sinewave[232] = 32412;
	Sinewave[233] = 32441; Sinewave[234] = 32468; Sinewave[235] = 32495; Sinewave[236] = 32520; Sinewave[237] = 32544; Sinewave[238] = 32567; Sinewave[239] = 32588; Sinewave[240] = 32609;
	Sinewave[241] = 32628; Sinewave[242] = 32646; Sinewave[243] = 32662; Sinewave[244] = 32678; Sinewave[245] = 32692; Sinewave[246] = 32705; Sinewave[247] = 32717; Sinewave[248] = 32727;
	Sinewave[249] = 32736; Sinewave[250] = 32744; Sinewave[251] = 32751; Sinewave[252] = 32757; Sinewave[253] = 32761; Sinewave[254] = 32764; Sinewave[255] = 32766; Sinewave[256] = 32767;
	Sinewave[257] = 32766; Sinewave[258] = 32764; Sinewave[259] = 32761; Sinewave[260] = 32757; Sinewave[261] = 32751; Sinewave[262] = 32744; Sinewave[263] = 32736; Sinewave[264] = 32727;
	Sinewave[265] = 32717; Sinewave[266] = 32705; Sinewave[267] = 32692; Sinewave[268] = 32678; Sinewave[269] = 32662; Sinewave[270] = 32646; Sinewave[271] = 32628; Sinewave[272] = 32609;
	Sinewave[273] = 32588; Sinewave[274] = 32567; Sinewave[275] = 32544; Sinewave[276] = 32520; Sinewave[277] = 32495; Sinewave[278] = 32468; Sinewave[279] = 32441; Sinewave[280] = 32412;
	Sinewave[281] = 32382; Sinewave[282] = 32350; Sinewave[283] = 32318; Sinewave[284] = 32284; Sinewave[285] = 32249; Sinewave[286] = 32213; Sinewave[287] = 32176; Sinewave[288] = 32137;
	Sinewave[289] = 32097; Sinewave[290] = 32056; Sinewave[291] = 32014; Sinewave[292] = 31970; Sinewave[293] = 31926; Sinewave[294] = 31880; Sinewave[295] = 31833; Sinewave[296] = 31785;
	Sinewave[297] = 31735; Sinewave[298] = 31684; Sinewave[299] = 31633; Sinewave[300] = 31580; Sinewave[301] = 31525; Sinewave[302] = 31470; Sinewave[303] = 31413; Sinewave[304] = 31356;
	Sinewave[305] = 31297; Sinewave[306] = 31236; Sinewave[307] = 31175; Sinewave[308] = 31113; Sinewave[309] = 31049; Sinewave[310] = 30984; Sinewave[311] = 30918; Sinewave[312] = 30851;
	Sinewave[313] = 30783; Sinewave[314] = 30713; Sinewave[315] = 30643; Sinewave[316] = 30571; Sinewave[317] = 30498; Sinewave[318] = 30424; Sinewave[319] = 30349; Sinewave[320] = 30272;
	Sinewave[321] = 30195; Sinewave[322] = 30116; Sinewave[323] = 30036; Sinewave[324] = 29955; Sinewave[325] = 29873; Sinewave[326] = 29790; Sinewave[327] = 29706; Sinewave[328] = 29621;
	Sinewave[329] = 29534; Sinewave[330] = 29446; Sinewave[331] = 29358; Sinewave[332] = 29268; Sinewave[333] = 29177; Sinewave[334] = 29085; Sinewave[335] = 28992; Sinewave[336] = 28897;
	Sinewave[337] = 28802; Sinewave[338] = 28706; Sinewave[339] = 28608; Sinewave[340] = 28510; Sinewave[341] = 28410; Sinewave[342] = 28309; Sinewave[343] = 28208; Sinewave[344] = 28105;
	Sinewave[345] = 28001; Sinewave[346] = 27896; Sinewave[347] = 27790; Sinewave[348] = 27683; Sinewave[349] = 27575; Sinewave[350] = 27466; Sinewave[351] = 27355; Sinewave[352] = 27244;
	Sinewave[353] = 27132; Sinewave[354] = 27019; Sinewave[355] = 26905; Sinewave[356] = 26789; Sinewave[357] = 26673; Sinewave[358] = 26556; Sinewave[359] = 26437; Sinewave[360] = 26318;
	Sinewave[361] = 26198; Sinewave[362] = 26077; Sinewave[363] = 25954; Sinewave[364] = 25831; Sinewave[365] = 25707; Sinewave[366] = 25582; Sinewave[367] = 25456; Sinewave[368] = 25329;
	Sinewave[369] = 25201; Sinewave[370] = 25072; Sinewave[371] = 24942; Sinewave[372] = 24811; Sinewave[373] = 24679; Sinewave[374] = 24546; Sinewave[375] = 24413; Sinewave[376] = 24278;
	Sinewave[377] = 24143; Sinewave[378] = 24006; Sinewave[379] = 23869; Sinewave[380] = 23731; Sinewave[381] = 23592; Sinewave[382] = 23452; Sinewave[383] = 23311; Sinewave[384] = 23169;
	Sinewave[385] = 23027; Sinewave[386] = 22883; Sinewave[387] = 22739; Sinewave[388] = 22594; Sinewave[389] = 22448; Sinewave[390] = 22301; Sinewave[391] = 22153; Sinewave[392] = 22004;
	Sinewave[393] = 21855; Sinewave[394] = 21705; Sinewave[395] = 21554; Sinewave[396] = 21402; Sinewave[397] = 21249; Sinewave[398] = 21096; Sinewave[399] = 20942; Sinewave[400] = 20787;
	Sinewave[401] = 20631; Sinewave[402] = 20474; Sinewave[403] = 20317; Sinewave[404] = 20159; Sinewave[405] = 20000; Sinewave[406] = 19840; Sinewave[407] = 19680; Sinewave[408] = 19519;
	Sinewave[409] = 19357; Sinewave[410] = 19194; Sinewave[411] = 19031; Sinewave[412] = 18867; Sinewave[413] = 18702; Sinewave[414] = 18537; Sinewave[415] = 18371; Sinewave[416] = 18204;
	Sinewave[417] = 18036; Sinewave[418] = 17868; Sinewave[419] = 17699; Sinewave[420] = 17530; Sinewave[421] = 17360; Sinewave[422] = 17189; Sinewave[423] = 17017; Sinewave[424] = 16845;
	Sinewave[425] = 16672; Sinewave[426] = 16499; Sinewave[427] = 16325; Sinewave[428] = 16150; Sinewave[429] = 15975; Sinewave[430] = 15799; Sinewave[431] = 15623; Sinewave[432] = 15446;
	Sinewave[433] = 15268; Sinewave[434] = 15090; Sinewave[435] = 14911; Sinewave[436] = 14732; Sinewave[437] = 14552; Sinewave[438] = 14372; Sinewave[439] = 14191; Sinewave[440] = 14009;
	Sinewave[441] = 13827; Sinewave[442] = 13645; Sinewave[443] = 13462; Sinewave[444] = 13278; Sinewave[445] = 13094; Sinewave[446] = 12909; Sinewave[447] = 12724; Sinewave[448] = 12539;
	Sinewave[449] = 12353; Sinewave[450] = 12166; Sinewave[451] = 11980; Sinewave[452] = 11792; Sinewave[453] = 11604; Sinewave[454] = 11416; Sinewave[455] = 11227; Sinewave[456] = 11038;
	Sinewave[457] = 10849; Sinewave[458] = 10659; Sinewave[459] = 10469; Sinewave[460] = 10278; Sinewave[461] = 10087; Sinewave[462] = 9895; Sinewave[463] = 9703; Sinewave[464] = 9511;
	Sinewave[465] = 9319; Sinewave[466] = 9126; Sinewave[467] = 8932; Sinewave[468] = 8739; Sinewave[469] = 8545; Sinewave[470] = 8351; Sinewave[471] = 8156; Sinewave[472] = 7961;
	Sinewave[473] = 7766; Sinewave[474] = 7571; Sinewave[475] = 7375; Sinewave[476] = 7179; Sinewave[477] = 6982; Sinewave[478] = 6786; Sinewave[479] = 6589; Sinewave[480] = 6392;
	Sinewave[481] = 6195; Sinewave[482] = 5997; Sinewave[483] = 5799; Sinewave[484] = 5601; Sinewave[485] = 5403; Sinewave[486] = 5205; Sinewave[487] = 5006; Sinewave[488] = 4807;
	Sinewave[489] = 4608; Sinewave[490] = 4409; Sinewave[491] = 4210; Sinewave[492] = 4011; Sinewave[493] = 3811; Sinewave[494] = 3611; Sinewave[495] = 3411; Sinewave[496] = 3211;
	Sinewave[497] = 3011; Sinewave[498] = 2811; Sinewave[499] = 2610; Sinewave[500] = 2410; Sinewave[501] = 2209; Sinewave[502] = 2009; Sinewave[503] = 1808; Sinewave[504] = 1607;
	Sinewave[505] = 1406; Sinewave[506] = 1206; Sinewave[507] = 1005; Sinewave[508] = 804; Sinewave[509] = 603; Sinewave[510] = 402; Sinewave[511] = 201; Sinewave[512] = 0;
	Sinewave[513] = -201; Sinewave[514] = -402; Sinewave[515] = -603; Sinewave[516] = -804; Sinewave[517] = -1005; Sinewave[518] = -1206; Sinewave[519] = -1406; Sinewave[520] = -1607;
	Sinewave[521] = -1808; Sinewave[522] = -2009; Sinewave[523] = -2209; Sinewave[524] = -2410; Sinewave[525] = -2610; Sinewave[526] = -2811; Sinewave[527] = -3011; Sinewave[528] = -3211;
	Sinewave[529] = -3411; Sinewave[530] = -3611; Sinewave[531] = -3811; Sinewave[532] = -4011; Sinewave[533] = -4210; Sinewave[534] = -4409; Sinewave[535] = -4608; Sinewave[536] = -4807;
	Sinewave[537] = -5006; Sinewave[538] = -5205; Sinewave[539] = -5403; Sinewave[540] = -5601; Sinewave[541] = -5799; Sinewave[542] = -5997; Sinewave[543] = -6195; Sinewave[544] = -6392;
	Sinewave[545] = -6589; Sinewave[546] = -6786; Sinewave[547] = -6982; Sinewave[548] = -7179; Sinewave[549] = -7375; Sinewave[550] = -7571; Sinewave[551] = -7766; Sinewave[552] = -7961;
	Sinewave[553] = -8156; Sinewave[554] = -8351; Sinewave[555] = -8545; Sinewave[556] = -8739; Sinewave[557] = -8932; Sinewave[558] = -9126; Sinewave[559] = -9319; Sinewave[560] = -9511;
	Sinewave[561] = -9703; Sinewave[562] = -9895; Sinewave[563] = -10087; Sinewave[564] = -10278; Sinewave[565] = -10469; Sinewave[566] = -10659; Sinewave[567] = -10849; Sinewave[568] = -11038;
	Sinewave[569] = -11227; Sinewave[570] = -11416; Sinewave[571] = -11604; Sinewave[572] = -11792; Sinewave[573] = -11980; Sinewave[574] = -12166; Sinewave[575] = -12353; Sinewave[576] = -12539;
	Sinewave[577] = -12724; Sinewave[578] = -12909; Sinewave[579] = -13094; Sinewave[580] = -13278; Sinewave[581] = -13462; Sinewave[582] = -13645; Sinewave[583] = -13827; Sinewave[584] = -14009;
	Sinewave[585] = -14191; Sinewave[586] = -14372; Sinewave[587] = -14552; Sinewave[588] = -14732; Sinewave[589] = -14911; Sinewave[590] = -15090; Sinewave[591] = -15268; Sinewave[592] = -15446;
	Sinewave[593] = -15623; Sinewave[594] = -15799; Sinewave[595] = -15975; Sinewave[596] = -16150; Sinewave[597] = -16325; Sinewave[598] = -16499; Sinewave[599] = -16672; Sinewave[600] = -16845;
	Sinewave[601] = -17017; Sinewave[602] = -17189; Sinewave[603] = -17360; Sinewave[604] = -17530; Sinewave[605] = -17699; Sinewave[606] = -17868; Sinewave[607] = -18036; Sinewave[608] = -18204;
	Sinewave[609] = -18371; Sinewave[610] = -18537; Sinewave[611] = -18702; Sinewave[612] = -18867; Sinewave[613] = -19031; Sinewave[614] = -19194; Sinewave[615] = -19357; Sinewave[616] = -19519;
	Sinewave[617] = -19680; Sinewave[618] = -19840; Sinewave[619] = -20000; Sinewave[620] = -20159; Sinewave[621] = -20317; Sinewave[622] = -20474; Sinewave[623] = -20631; Sinewave[624] = -20787;
	Sinewave[625] = -20942; Sinewave[626] = -21096; Sinewave[627] = -21249; Sinewave[628] = -21402; Sinewave[629] = -21554; Sinewave[630] = -21705; Sinewave[631] = -21855; Sinewave[632] = -22004;
	Sinewave[633] = -22153; Sinewave[634] = -22301; Sinewave[635] = -22448; Sinewave[636] = -22594; Sinewave[637] = -22739; Sinewave[638] = -22883; Sinewave[639] = -23027; Sinewave[640] = -23169;
	Sinewave[641] = -23311; Sinewave[642] = -23452; Sinewave[643] = -23592; Sinewave[644] = -23731; Sinewave[645] = -23869; Sinewave[646] = -24006; Sinewave[647] = -24143; Sinewave[648] = -24278;
	Sinewave[649] = -24413; Sinewave[650] = -24546; Sinewave[651] = -24679; Sinewave[652] = -24811; Sinewave[653] = -24942; Sinewave[654] = -25072; Sinewave[655] = -25201; Sinewave[656] = -25329;
	Sinewave[657] = -25456; Sinewave[658] = -25582; Sinewave[659] = -25707; Sinewave[660] = -25831; Sinewave[661] = -25954; Sinewave[662] = -26077; Sinewave[663] = -26198; Sinewave[664] = -26318;
	Sinewave[665] = -26437; Sinewave[666] = -26556; Sinewave[667] = -26673; Sinewave[668] = -26789; Sinewave[669] = -26905; Sinewave[670] = -27019; Sinewave[671] = -27132; Sinewave[672] = -27244;
	Sinewave[673] = -27355; Sinewave[674] = -27466; Sinewave[675] = -27575; Sinewave[676] = -27683; Sinewave[677] = -27790; Sinewave[678] = -27896; Sinewave[679] = -28001; Sinewave[680] = -28105;
	Sinewave[681] = -28208; Sinewave[682] = -28309; Sinewave[683] = -28410; Sinewave[684] = -28510; Sinewave[685] = -28608; Sinewave[686] = -28706; Sinewave[687] = -28802; Sinewave[688] = -28897;
	Sinewave[689] = -28992; Sinewave[690] = -29085; Sinewave[691] = -29177; Sinewave[692] = -29268; Sinewave[693] = -29358; Sinewave[694] = -29446; Sinewave[695] = -29534; Sinewave[696] = -29621;
	Sinewave[697] = -29706; Sinewave[698] = -29790; Sinewave[699] = -29873; Sinewave[700] = -29955; Sinewave[701] = -30036; Sinewave[702] = -30116; Sinewave[703] = -30195; Sinewave[704] = -30272;
	Sinewave[705] = -30349; Sinewave[706] = -30424; Sinewave[707] = -30498; Sinewave[708] = -30571; Sinewave[709] = -30643; Sinewave[710] = -30713; Sinewave[711] = -30783; Sinewave[712] = -30851;
	Sinewave[713] = -30918; Sinewave[714] = -30984; Sinewave[715] = -31049; Sinewave[716] = -31113; Sinewave[717] = -31175; Sinewave[718] = -31236; Sinewave[719] = -31297; Sinewave[720] = -31356;
	Sinewave[721] = -31413; Sinewave[722] = -31470; Sinewave[723] = -31525; Sinewave[724] = -31580; Sinewave[725] = -31633; Sinewave[726] = -31684; Sinewave[727] = -31735; Sinewave[728] = -31785;
	Sinewave[729] = -31833; Sinewave[730] = -31880; Sinewave[731] = -31926; Sinewave[732] = -31970; Sinewave[733] = -32014; Sinewave[734] = -32056; Sinewave[735] = -32097; Sinewave[736] = -32137;
	Sinewave[737] = -32176; Sinewave[738] = -32213; Sinewave[739] = -32249; Sinewave[740] = -32284; Sinewave[741] = -32318; Sinewave[742] = -32350; Sinewave[743] = -32382; Sinewave[744] = -32412;
	Sinewave[745] = -32441; Sinewave[746] = -32468; Sinewave[747] = -32495; Sinewave[748] = -32520; Sinewave[749] = -32544; Sinewave[750] = -32567; Sinewave[751] = -32588; Sinewave[752] = -32609;
	Sinewave[753] = -32628; Sinewave[754] = -32646; Sinewave[755] = -32662; Sinewave[756] = -32678; Sinewave[757] = -32692; Sinewave[758] = -32705; Sinewave[759] = -32717; Sinewave[760] = -32727;
	Sinewave[761] = -32736; Sinewave[762] = -32744; Sinewave[763] = -32751; Sinewave[764] = -32757; Sinewave[765] = -32761; Sinewave[766] = -32764; Sinewave[767] = -32766; Sinewave[768] = -32767;
	Sinewave[769] = -32766; Sinewave[770] = -32764; Sinewave[771] = -32761; Sinewave[772] = -32757; Sinewave[773] = -32751; Sinewave[774] = -32744; Sinewave[775] = -32736; Sinewave[776] = -32727;
	Sinewave[777] = -32717; Sinewave[778] = -32705; Sinewave[779] = -32692; Sinewave[780] = -32678; Sinewave[781] = -32662; Sinewave[782] = -32646; Sinewave[783] = -32628; Sinewave[784] = -32609;
	Sinewave[785] = -32588; Sinewave[786] = -32567; Sinewave[787] = -32544; Sinewave[788] = -32520; Sinewave[789] = -32495; Sinewave[790] = -32468; Sinewave[791] = -32441; Sinewave[792] = -32412;
	Sinewave[793] = -32382; Sinewave[794] = -32350; Sinewave[795] = -32318; Sinewave[796] = -32284; Sinewave[797] = -32249; Sinewave[798] = -32213; Sinewave[799] = -32176; Sinewave[800] = -32137;
	Sinewave[801] = -32097; Sinewave[802] = -32056; Sinewave[803] = -32014; Sinewave[804] = -31970; Sinewave[805] = -31926; Sinewave[806] = -31880; Sinewave[807] = -31833; Sinewave[808] = -31785;
	Sinewave[809] = -31735; Sinewave[810] = -31684; Sinewave[811] = -31633; Sinewave[812] = -31580; Sinewave[813] = -31525; Sinewave[814] = -31470; Sinewave[815] = -31413; Sinewave[816] = -31356;
	Sinewave[817] = -31297; Sinewave[818] = -31236; Sinewave[819] = -31175; Sinewave[820] = -31113; Sinewave[821] = -31049; Sinewave[822] = -30984; Sinewave[823] = -30918; Sinewave[824] = -30851;
	Sinewave[825] = -30783; Sinewave[826] = -30713; Sinewave[827] = -30643; Sinewave[828] = -30571; Sinewave[829] = -30498; Sinewave[830] = -30424; Sinewave[831] = -30349; Sinewave[832] = -30272;
	Sinewave[833] = -30195; Sinewave[834] = -30116; Sinewave[835] = -30036; Sinewave[836] = -29955; Sinewave[837] = -29873; Sinewave[838] = -29790; Sinewave[839] = -29706; Sinewave[840] = -29621;
	Sinewave[841] = -29534; Sinewave[842] = -29446; Sinewave[843] = -29358; Sinewave[844] = -29268; Sinewave[845] = -29177; Sinewave[846] = -29085; Sinewave[847] = -28992; Sinewave[848] = -28897;
	Sinewave[849] = -28802; Sinewave[850] = -28706; Sinewave[851] = -28608; Sinewave[852] = -28510; Sinewave[853] = -28410; Sinewave[854] = -28309; Sinewave[855] = -28208; Sinewave[856] = -28105;
	Sinewave[857] = -28001; Sinewave[858] = -27896; Sinewave[859] = -27790; Sinewave[860] = -27683; Sinewave[861] = -27575; Sinewave[862] = -27466; Sinewave[863] = -27355; Sinewave[864] = -27244;
	Sinewave[865] = -27132; Sinewave[866] = -27019; Sinewave[867] = -26905; Sinewave[868] = -26789; Sinewave[869] = -26673; Sinewave[870] = -26556; Sinewave[871] = -26437; Sinewave[872] = -26318;
	Sinewave[873] = -26198; Sinewave[874] = -26077; Sinewave[875] = -25954; Sinewave[876] = -25831; Sinewave[877] = -25707; Sinewave[878] = -25582; Sinewave[879] = -25456; Sinewave[880] = -25329;
	Sinewave[881] = -25201; Sinewave[882] = -25072; Sinewave[883] = -24942; Sinewave[884] = -24811; Sinewave[885] = -24679; Sinewave[886] = -24546; Sinewave[887] = -24413; Sinewave[888] = -24278;
	Sinewave[889] = -24143; Sinewave[890] = -24006; Sinewave[891] = -23869; Sinewave[892] = -23731; Sinewave[893] = -23592; Sinewave[894] = -23452; Sinewave[895] = -23311; Sinewave[896] = -23169;
	Sinewave[897] = -23027; Sinewave[898] = -22883; Sinewave[899] = -22739; Sinewave[900] = -22594; Sinewave[901] = -22448; Sinewave[902] = -22301; Sinewave[903] = -22153; Sinewave[904] = -22004;
	Sinewave[905] = -21855; Sinewave[906] = -21705; Sinewave[907] = -21554; Sinewave[908] = -21402; Sinewave[909] = -21249; Sinewave[910] = -21096; Sinewave[911] = -20942; Sinewave[912] = -20787;
	Sinewave[913] = -20631; Sinewave[914] = -20474; Sinewave[915] = -20317; Sinewave[916] = -20159; Sinewave[917] = -20000; Sinewave[918] = -19840; Sinewave[919] = -19680; Sinewave[920] = -19519;
	Sinewave[921] = -19357; Sinewave[922] = -19194; Sinewave[923] = -19031; Sinewave[924] = -18867; Sinewave[925] = -18702; Sinewave[926] = -18537; Sinewave[927] = -18371; Sinewave[928] = -18204;
	Sinewave[929] = -18036; Sinewave[930] = -17868; Sinewave[931] = -17699; Sinewave[932] = -17530; Sinewave[933] = -17360; Sinewave[934] = -17189; Sinewave[935] = -17017; Sinewave[936] = -16845;
	Sinewave[937] = -16672; Sinewave[938] = -16499; Sinewave[939] = -16325; Sinewave[940] = -16150; Sinewave[941] = -15975; Sinewave[942] = -15799; Sinewave[943] = -15623; Sinewave[944] = -15446;
	Sinewave[945] = -15268; Sinewave[946] = -15090; Sinewave[947] = -14911; Sinewave[948] = -14732; Sinewave[949] = -14552; Sinewave[950] = -14372; Sinewave[951] = -14191; Sinewave[952] = -14009;
	Sinewave[953] = -13827; Sinewave[954] = -13645; Sinewave[955] = -13462; Sinewave[956] = -13278; Sinewave[957] = -13094; Sinewave[958] = -12909; Sinewave[959] = -12724; Sinewave[960] = -12539;
	Sinewave[961] = -12353; Sinewave[962] = -12166; Sinewave[963] = -11980; Sinewave[964] = -11792; Sinewave[965] = -11604; Sinewave[966] = -11416; Sinewave[967] = -11227; Sinewave[968] = -11038;
	Sinewave[969] = -10849; Sinewave[970] = -10659; Sinewave[971] = -10469; Sinewave[972] = -10278; Sinewave[973] = -10087; Sinewave[974] = -9895; Sinewave[975] = -9703; Sinewave[976] = -9511;
	Sinewave[977] = -9319; Sinewave[978] = -9126; Sinewave[979] = -8932; Sinewave[980] = -8739; Sinewave[981] = -8545; Sinewave[982] = -8351; Sinewave[983] = -8156; Sinewave[984] = -7961;
	Sinewave[985] = -7766; Sinewave[986] = -7571; Sinewave[987] = -7375; Sinewave[988] = -7179; Sinewave[989] = -6982; Sinewave[990] = -6786; Sinewave[991] = -6589; Sinewave[992] = -6392;
	Sinewave[993] = -6195; Sinewave[994] = -5997; Sinewave[995] = -5799; Sinewave[996] = -5601; Sinewave[997] = -5403; Sinewave[998] = -5205; Sinewave[999] = -5006; Sinewave[1000] = -4807;
	Sinewave[1001] = -4608; Sinewave[1002] = -4409; Sinewave[1003] = -4210; Sinewave[1004] = -4011; Sinewave[1005] = -3811; Sinewave[1006] = -3611; Sinewave[1007] = -3411; Sinewave[1008] = -3211;
	Sinewave[1009] = -3011; Sinewave[1010] = -2811; Sinewave[1011] = -2610; Sinewave[1012] = -2410; Sinewave[1013] = -2209; Sinewave[1014] = -2009; Sinewave[1015] = -1808; Sinewave[1016] = -1607;
	Sinewave[1017] = -1406; Sinewave[1018] = -1206; Sinewave[1019] = -1005; Sinewave[1020] = -804; Sinewave[1021] = -603; Sinewave[1022] = -402; Sinewave[1023] = -201;
}

static const int sqrttable[] = {
	0, 16, 22, 27, 32, 35, 39, 42, 45, 48, 50, 53, 55, 57,
	59, 61, 64, 65, 67, 69, 71, 73, 75, 76, 78, 80, 81, 83,
	84, 86, 87, 89, 90, 91, 93, 94, 96, 97, 98, 99, 101, 102,
	103, 104, 106, 107, 108, 109, 110, 112, 113, 114, 115, 116, 117, 118,
	119, 120, 121, 122, 123, 124, 125, 126, 128, 128, 129, 130, 131, 132,
	133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 144, 145,
	146, 147, 148, 149, 150, 150, 151, 152, 153, 154, 155, 155, 156, 157,
	158, 159, 160, 160, 161, 162, 163, 163, 164, 165, 166, 167, 167, 168,
	169, 170, 170, 171, 172, 173, 173, 174, 175, 176, 176, 177, 178, 178,
	179, 180, 181, 181, 182, 183, 183, 184, 185, 185, 186, 187, 187, 188,
	189, 189, 190, 191, 192, 192, 193, 193, 194, 195, 195, 196, 197, 197,
	198, 199, 199, 200, 201, 201, 202, 203, 203, 204, 204, 205, 206, 206,
	207, 208, 208, 209, 209, 210, 211, 211, 212, 212, 213, 214, 214, 215,
	215, 216, 217, 217, 218, 218, 219, 219, 220, 221, 221, 222, 222, 223,
	224, 224, 225, 225, 226, 226, 227, 227, 228, 229, 229, 230, 230, 231,
	231, 232, 232, 233, 234, 234, 235, 235, 236, 236, 237, 237, 238, 238,
	239, 240, 240, 241, 241, 242, 242, 243, 243, 244, 244, 245, 245, 246,
	246, 247, 247, 248, 248, 249, 249, 250, 250, 251, 251, 252, 252, 253,
	253, 254, 254, 255
};


short cosine(int rad)
{
	int angle = rad+256;
	while(angle >= 1024)
		angle -= 1024;

	return Sinewave[angle];
}

int absolute(int real, int imag)
{
//	printf("Abs... ");
//	fflush(stdout);
	int temp = sqrt_int(real*real + imag*imag);
//	printf("done\n");
//	fflush(stdout);
	return temp;
}

int sqrt_int(int x)
{
	int xn;

	if (x >= 0x10000) {
		if (x >= 0x1000000) {
			if (x >= 0x10000000) {
				if (x >= 0x40000000) {
					if (x >= 2147483647) {
						return 65535;
					}

					xn = sqrttable[x >> 24] << 8;
				} else {
					xn = sqrttable[x >> 22] << 7;
				}
			} else {
				if (x >= 0x4000000) {
					xn = sqrttable[x >> 20] << 6;
				} else {
					xn = sqrttable[x >> 18] << 5;
				}
			}

			xn = (xn + 1 + (x / xn)) >> 1;
			xn = (xn + 1 + (x / xn)) >> 1;

			return ((xn * xn) > x) ? --xn : xn;
		} else {
			if (x >= 0x100000) {
				if (x >= 0x400000) {
					xn = sqrttable[x >> 16] << 4;
				} else {
					xn = sqrttable[x >> 14] << 3;
				}
			} else {
				if (x >= 0x40000) {
					xn = sqrttable[x >> 12] << 2;
				} else {
					xn = sqrttable[x >> 10] << 1;
				}
			}

			xn = (xn + 1 + (x / xn)) >> 1;

			return ((xn * xn) > x) ? --xn : xn;
		}
	} else {
		if (x >= 0x100) {
			if (x >= 0x1000) {
				if (x >= 0x4000) {
					xn = (sqrttable[x >> 8] ) + 1;
				} else {
					xn = (sqrttable[x >> 6] >> 1) + 1;
				}
			} else {
				if (x >= 0x400) {
					xn = (sqrttable[x >> 4] >> 2) + 1;
				} else {
					xn = (sqrttable[x >> 2] >> 3) + 1;
				}
			}

			return ((xn * xn) > x) ? --xn : xn;
		} else {
			if (x >= 0) {
				return sqrttable[x] >> 4;
			} else {
				return -1; // negative argument...
			}
		}
	}
}

int logTwo(int n) {
	int pos = 0;
	if (n >= 1<<16) { n >>= 16; pos += 16; }
	if (n >= 1<< 8) { n >>=  8; pos +=  8; }
	if (n >= 1<< 4) { n >>=  4; pos +=  4; }
	if (n >= 1<< 2) { n >>=  2; pos +=  2; }
	if (n >= 1<< 1) {           pos +=  1; }
	return ((n == 0) ? (-1) : pos);
}

int ln(int n)
{
	//long tmp = (int64_t)floorLog2(n) * 1443;
	return logTwo(n) * 25 / 36;
}


int C_shipLength(int shipIndex)
{
	int shipLength = 3;
	switch (shipIndex)
	{
	case 0: shipLength = 5; break;
	case 1: shipLength = 4; break;
	case 4: shipLength = 2; break;
	}
	return shipLength;
}

bool C_outOfBoard(int x, int y)
{
	return (x < 0 ||  y < 0 || x > BOARD_SIZE - 1 || y > BOARD_SIZE - 1);
}
