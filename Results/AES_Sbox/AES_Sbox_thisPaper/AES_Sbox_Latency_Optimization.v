// Auto-generated from f0-f7_bool_func_test_11.24_TwoTree_revised.v
// Converted to the AES_Sbox_Latency_Optimization.v t-gate naming style.
//
// Verified bit convention:
//   X0 is input bit 7 (MSB), X7 is input bit 0 (LSB).
//   Y0 is output bit 7 (MSB), Y7 is output bit 0 (LSB).
// The complete 256-entry truth table matches the standard AES S-box.
// Alias-only wires in the source are removed; every tN below is a real
// NOT, NAND, AND, or XOR gate from the active source logic.

module AES_SBOX_TGATES(
    input  wire X0, X1, X2, X3, X4, X5, X6, X7,
    output wire Y0, Y1, Y2, Y3, Y4, Y5, Y6, Y7
);

  // Gate count: 1429 (NOT=64, NAND=166, AND=943, XOR2=256)
  // Maximum reported logic depth: 9

  wire t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15;
  wire t16, t17, t18, t19, t20, t21, t22, t23, t24, t25, t26, t27, t28, t29, t30, t31;
  wire t32, t33, t34, t35, t36, t37, t38, t39, t40, t41, t42, t43, t44, t45, t46, t47;
  wire t48, t49, t50, t51, t52, t53, t54, t55, t56, t57, t58, t59, t60, t61, t62, t63;
  wire t64, t65, t66, t67, t68, t69, t70, t71, t72, t73, t74, t75, t76, t77, t78, t79;
  wire t80, t81, t82, t83, t84, t85, t86, t87, t88, t89, t90, t91, t92, t93, t94, t95;
  wire t96, t97, t98, t99, t100, t101, t102, t103, t104, t105, t106, t107, t108, t109, t110, t111;
  wire t112, t113, t114, t115, t116, t117, t118, t119, t120, t121, t122, t123, t124, t125, t126, t127;
  wire t128, t129, t130, t131, t132, t133, t134, t135, t136, t137, t138, t139, t140, t141, t142, t143;
  wire t144, t145, t146, t147, t148, t149, t150, t151, t152, t153, t154, t155, t156, t157, t158, t159;
  wire t160, t161, t162, t163, t164, t165, t166, t167, t168, t169, t170, t171, t172, t173, t174, t175;
  wire t176, t177, t178, t179, t180, t181, t182, t183, t184, t185, t186, t187, t188, t189, t190, t191;
  wire t192, t193, t194, t195, t196, t197, t198, t199, t200, t201, t202, t203, t204, t205, t206, t207;
  wire t208, t209, t210, t211, t212, t213, t214, t215, t216, t217, t218, t219, t220, t221, t222, t223;
  wire t224, t225, t226, t227, t228, t229, t230, t231, t232, t233, t234, t235, t236, t237, t238, t239;
  wire t240, t241, t242, t243, t244, t245, t246, t247, t248, t249, t250, t251, t252, t253, t254, t255;
  wire t256, t257, t258, t259, t260, t261, t262, t263, t264, t265, t266, t267, t268, t269, t270, t271;
  wire t272, t273, t274, t275, t276, t277, t278, t279, t280, t281, t282, t283, t284, t285, t286, t287;
  wire t288, t289, t290, t291, t292, t293, t294, t295, t296, t297, t298, t299, t300, t301, t302, t303;
  wire t304, t305, t306, t307, t308, t309, t310, t311, t312, t313, t314, t315, t316, t317, t318, t319;
  wire t320, t321, t322, t323, t324, t325, t326, t327, t328, t329, t330, t331, t332, t333, t334, t335;
  wire t336, t337, t338, t339, t340, t341, t342, t343, t344, t345, t346, t347, t348, t349, t350, t351;
  wire t352, t353, t354, t355, t356, t357, t358, t359, t360, t361, t362, t363, t364, t365, t366, t367;
  wire t368, t369, t370, t371, t372, t373, t374, t375, t376, t377, t378, t379, t380, t381, t382, t383;
  wire t384, t385, t386, t387, t388, t389, t390, t391, t392, t393, t394, t395, t396, t397, t398, t399;
  wire t400, t401, t402, t403, t404, t405, t406, t407, t408, t409, t410, t411, t412, t413, t414, t415;
  wire t416, t417, t418, t419, t420, t421, t422, t423, t424, t425, t426, t427, t428, t429, t430, t431;
  wire t432, t433, t434, t435, t436, t437, t438, t439, t440, t441, t442, t443, t444, t445, t446, t447;
  wire t448, t449, t450, t451, t452, t453, t454, t455, t456, t457, t458, t459, t460, t461, t462, t463;
  wire t464, t465, t466, t467, t468, t469, t470, t471, t472, t473, t474, t475, t476, t477, t478, t479;
  wire t480, t481, t482, t483, t484, t485, t486, t487, t488, t489, t490, t491, t492, t493, t494, t495;
  wire t496, t497, t498, t499, t500, t501, t502, t503, t504, t505, t506, t507, t508, t509, t510, t511;
  wire t512, t513, t514, t515, t516, t517, t518, t519, t520, t521, t522, t523, t524, t525, t526, t527;
  wire t528, t529, t530, t531, t532, t533, t534, t535, t536, t537, t538, t539, t540, t541, t542, t543;
  wire t544, t545, t546, t547, t548, t549, t550, t551, t552, t553, t554, t555, t556, t557, t558, t559;
  wire t560, t561, t562, t563, t564, t565, t566, t567, t568, t569, t570, t571, t572, t573, t574, t575;
  wire t576, t577, t578, t579, t580, t581, t582, t583, t584, t585, t586, t587, t588, t589, t590, t591;
  wire t592, t593, t594, t595, t596, t597, t598, t599, t600, t601, t602, t603, t604, t605, t606, t607;
  wire t608, t609, t610, t611, t612, t613, t614, t615, t616, t617, t618, t619, t620, t621, t622, t623;
  wire t624, t625, t626, t627, t628, t629, t630, t631, t632, t633, t634, t635, t636, t637, t638, t639;
  wire t640, t641, t642, t643, t644, t645, t646, t647, t648, t649, t650, t651, t652, t653, t654, t655;
  wire t656, t657, t658, t659, t660, t661, t662, t663, t664, t665, t666, t667, t668, t669, t670, t671;
  wire t672, t673, t674, t675, t676, t677, t678, t679, t680, t681, t682, t683, t684, t685, t686, t687;
  wire t688, t689, t690, t691, t692, t693, t694, t695, t696, t697, t698, t699, t700, t701, t702, t703;
  wire t704, t705, t706, t707, t708, t709, t710, t711, t712, t713, t714, t715, t716, t717, t718, t719;
  wire t720, t721, t722, t723, t724, t725, t726, t727, t728, t729, t730, t731, t732, t733, t734, t735;
  wire t736, t737, t738, t739, t740, t741, t742, t743, t744, t745, t746, t747, t748, t749, t750, t751;
  wire t752, t753, t754, t755, t756, t757, t758, t759, t760, t761, t762, t763, t764, t765, t766, t767;
  wire t768, t769, t770, t771, t772, t773, t774, t775, t776, t777, t778, t779, t780, t781, t782, t783;
  wire t784, t785, t786, t787, t788, t789, t790, t791, t792, t793, t794, t795, t796, t797, t798, t799;
  wire t800, t801, t802, t803, t804, t805, t806, t807, t808, t809, t810, t811, t812, t813, t814, t815;
  wire t816, t817, t818, t819, t820, t821, t822, t823, t824, t825, t826, t827, t828, t829, t830, t831;
  wire t832, t833, t834, t835, t836, t837, t838, t839, t840, t841, t842, t843, t844, t845, t846, t847;
  wire t848, t849, t850, t851, t852, t853, t854, t855, t856, t857, t858, t859, t860, t861, t862, t863;
  wire t864, t865, t866, t867, t868, t869, t870, t871, t872, t873, t874, t875, t876, t877, t878, t879;
  wire t880, t881, t882, t883, t884, t885, t886, t887, t888, t889, t890, t891, t892, t893, t894, t895;
  wire t896, t897, t898, t899, t900, t901, t902, t903, t904, t905, t906, t907, t908, t909, t910, t911;
  wire t912, t913, t914, t915, t916, t917, t918, t919, t920, t921, t922, t923, t924, t925, t926, t927;
  wire t928, t929, t930, t931, t932, t933, t934, t935, t936, t937, t938, t939, t940, t941, t942, t943;
  wire t944, t945, t946, t947, t948, t949, t950, t951, t952, t953, t954, t955, t956, t957, t958, t959;
  wire t960, t961, t962, t963, t964, t965, t966, t967, t968, t969, t970, t971, t972, t973, t974, t975;
  wire t976, t977, t978, t979, t980, t981, t982, t983, t984, t985, t986, t987, t988, t989, t990, t991;
  wire t992, t993, t994, t995, t996, t997, t998, t999, t1000, t1001, t1002, t1003, t1004, t1005, t1006, t1007;
  wire t1008, t1009, t1010, t1011, t1012, t1013, t1014, t1015, t1016, t1017, t1018, t1019, t1020, t1021, t1022, t1023;
  wire t1024, t1025, t1026, t1027, t1028, t1029, t1030, t1031, t1032, t1033, t1034, t1035, t1036, t1037, t1038, t1039;
  wire t1040, t1041, t1042, t1043, t1044, t1045, t1046, t1047, t1048, t1049, t1050, t1051, t1052, t1053, t1054, t1055;
  wire t1056, t1057, t1058, t1059, t1060, t1061, t1062, t1063, t1064, t1065, t1066, t1067, t1068, t1069, t1070, t1071;
  wire t1072, t1073, t1074, t1075, t1076, t1077, t1078, t1079, t1080, t1081, t1082, t1083, t1084, t1085, t1086, t1087;
  wire t1088, t1089, t1090, t1091, t1092, t1093, t1094, t1095, t1096, t1097, t1098, t1099, t1100, t1101, t1102, t1103;
  wire t1104, t1105, t1106, t1107, t1108, t1109, t1110, t1111, t1112, t1113, t1114, t1115, t1116, t1117, t1118, t1119;
  wire t1120, t1121, t1122, t1123, t1124, t1125, t1126, t1127, t1128, t1129, t1130, t1131, t1132, t1133, t1134, t1135;
  wire t1136, t1137, t1138, t1139, t1140, t1141, t1142, t1143, t1144, t1145, t1146, t1147, t1148, t1149, t1150, t1151;
  wire t1152, t1153, t1154, t1155, t1156, t1157, t1158, t1159, t1160, t1161, t1162, t1163, t1164, t1165, t1166, t1167;
  wire t1168, t1169, t1170, t1171, t1172, t1173, t1174, t1175, t1176, t1177, t1178, t1179, t1180, t1181, t1182, t1183;
  wire t1184, t1185, t1186, t1187, t1188, t1189, t1190, t1191, t1192, t1193, t1194, t1195, t1196, t1197, t1198, t1199;
  wire t1200, t1201, t1202, t1203, t1204, t1205, t1206, t1207, t1208, t1209, t1210, t1211, t1212, t1213, t1214, t1215;
  wire t1216, t1217, t1218, t1219, t1220, t1221, t1222, t1223, t1224, t1225, t1226, t1227, t1228, t1229, t1230, t1231;
  wire t1232, t1233, t1234, t1235, t1236, t1237, t1238, t1239, t1240, t1241, t1242, t1243, t1244, t1245, t1246, t1247;
  wire t1248, t1249, t1250, t1251, t1252, t1253, t1254, t1255, t1256, t1257, t1258, t1259, t1260, t1261, t1262, t1263;
  wire t1264, t1265, t1266, t1267, t1268, t1269, t1270, t1271, t1272, t1273, t1274, t1275, t1276, t1277, t1278, t1279;
  wire t1280, t1281, t1282, t1283, t1284, t1285, t1286, t1287, t1288, t1289, t1290, t1291, t1292, t1293, t1294, t1295;
  wire t1296, t1297, t1298, t1299, t1300, t1301, t1302, t1303, t1304, t1305, t1306, t1307, t1308, t1309, t1310, t1311;
  wire t1312, t1313, t1314, t1315, t1316, t1317, t1318, t1319, t1320, t1321, t1322, t1323, t1324, t1325, t1326, t1327;
  wire t1328, t1329, t1330, t1331, t1332, t1333, t1334, t1335, t1336, t1337, t1338, t1339, t1340, t1341, t1342, t1343;
  wire t1344, t1345, t1346, t1347, t1348, t1349, t1350, t1351, t1352, t1353, t1354, t1355, t1356, t1357, t1358, t1359;
  wire t1360, t1361, t1362, t1363, t1364, t1365, t1366, t1367, t1368, t1369, t1370, t1371, t1372, t1373, t1374, t1375;
  wire t1376, t1377, t1378, t1379, t1380, t1381, t1382, t1383, t1384, t1385, t1386, t1387, t1388, t1389, t1390, t1391;
  wire t1392, t1393, t1394, t1395, t1396, t1397, t1398, t1399, t1400, t1401, t1402, t1403, t1404, t1405, t1406, t1407;
  wire t1408, t1409, t1410, t1411, t1412, t1413, t1414, t1415, t1416, t1417, t1418, t1419, t1420, t1421, t1422, t1423;
  wire t1424, t1425, t1426, t1427, t1428;

  // depth 1  NAND(X2, X3)
  assign t0 = ~(X2 & X3);

  // depth 1  AND(X0, X1)
  assign t1 = (X0 & X1);

  // depth 2  AND(t1, X5)
  assign t2 = (t1 & X5);

  // depth 3  AND(t2, t0)
  assign t3 = (t2 & t0);

  // depth 0  NOT(X1)
  assign t4 = ~X1;

  // depth 0  NOT(X5)
  assign t5 = ~X5;

  // depth 1  AND(X3, X4)
  assign t6 = (X3 & X4);

  // depth 1  AND(X0, t4)
  assign t7 = (X0 & t4);

  // depth 2  AND(t6, t7)
  assign t8 = (t6 & t7);

  // depth 3  AND(t8, t5)
  assign t9 = (t8 & t5);

  // depth 1  NAND(X1, X3)
  assign t10 = ~(X1 & X3);

  // depth 1  AND(X0, X2)
  assign t11 = (X0 & X2);

  // depth 2  AND(t11, X6)
  assign t12 = (t11 & X6);

  // depth 3  AND(t12, t10)
  assign t13 = (t12 & t10);

  // depth 0  NOT(X2)
  assign t14 = ~X2;

  // depth 1  AND(X3, X4)
  assign t15 = (X3 & X4);

  // depth 1  AND(X0, X6)
  assign t16 = (X0 & X6);

  // depth 2  AND(t15, t16)
  assign t17 = (t15 & t16);

  // depth 3  AND(t17, t14)
  assign t18 = (t17 & t14);

  // depth 1  NAND(X0, X6)
  assign t19 = ~(X0 & X6);

  // depth 1  AND(X2, X5)
  assign t20 = (X2 & X5);

  // depth 2  AND(t20, t19)
  assign t21 = (t20 & t19);

  // depth 3  AND(t21, t4)
  assign t22 = (t21 & t4);

  // depth 1  NAND(X5, X6)
  assign t23 = ~(X5 & X6);

  // depth 1  AND(X0, X2)
  assign t24 = (X0 & X2);

  // depth 2  AND(t24, X3)
  assign t25 = (t24 & X3);

  // depth 3  AND(t25, t23)
  assign t26 = (t25 & t23);

  // depth 1  NAND(X1, X6)
  assign t27 = ~(X1 & X6);

  // depth 1  AND(X2, X3)
  assign t28 = (X2 & X3);

  // depth 2  AND(t28, X5)
  assign t29 = (t28 & X5);

  // depth 3  AND(t29, t27)
  assign t30 = (t29 & t27);

  // depth 1  NAND(X0, X5)
  assign t31 = ~(X0 & X5);

  // depth 1  AND(X2, X4)
  assign t32 = (X2 & X4);

  // depth 2  AND(t32, X6)
  assign t33 = (t32 & X6);

  // depth 3  AND(t33, t31)
  assign t34 = (t33 & t31);

  // depth 1  NAND(X2, X4)
  assign t35 = ~(X2 & X4);

  // depth 1  AND(X1, X6)
  assign t36 = (X1 & X6);

  // depth 2  AND(t36, t35)
  assign t37 = (t36 & t35);

  // depth 1  AND(X0, t5)
  assign t38 = (X0 & t5);

  // depth 3  AND(t37, t38)
  assign t39 = (t37 & t38);

  // depth 0  NOT(X0)
  assign t40 = ~X0;

  // depth 1  AND(X4, X5)
  assign t41 = (X4 & X5);

  // depth 1  AND(X3, X6)
  assign t42 = (X3 & X6);

  // depth 2  AND(t41, t42)
  assign t43 = (t41 & t42);

  // depth 1  AND(X1, t40)
  assign t44 = (X1 & t40);

  // depth 3  AND(t43, t44)
  assign t45 = (t43 & t44);

  // depth 0  NOT(X3)
  assign t46 = ~X3;

  // depth 0  NOT(X6)
  assign t47 = ~X6;

  // depth 1  AND(X4, X5)
  assign t48 = (X4 & X5);

  // depth 1  AND(X2, t46)
  assign t49 = (X2 & t46);

  // depth 2  AND(t48, t49)
  assign t50 = (t48 & t49);

  // depth 1  AND(X1, t47)
  assign t51 = (X1 & t47);

  // depth 3  AND(t50, t51)
  assign t52 = (t50 & t51);

  // depth 0  NOT(X7)
  assign t53 = ~X7;

  // depth 1  AND(X2, t46)
  assign t54 = (X2 & t46);

  // depth 2  AND(t54, t53)
  assign t55 = (t54 & t53);

  // depth 1  NAND(X2, X7)
  assign t56 = ~(X2 & X7);

  // depth 1  AND(X1, X3)
  assign t57 = (X1 & X3);

  // depth 2  AND(t57, t40)
  assign t58 = (t57 & t40);

  // depth 3  AND(t58, t56)
  assign t59 = (t58 & t56);

  // depth 1  AND(X4, t40)
  assign t60 = (X4 & t40);

  // depth 1  AND(X2, t4)
  assign t61 = (X2 & t4);

  // depth 2  AND(t60, t61)
  assign t62 = (t60 & t61);

  // depth 3  AND(t62, t53)
  assign t63 = (t62 & t53);

  // depth 0  NOT(X4)
  assign t64 = ~X4;

  // depth 1  AND(X7, t40)
  assign t65 = (X7 & t40);

  // depth 1  AND(X3, t4)
  assign t66 = (X3 & t4);

  // depth 2  AND(t65, t66)
  assign t67 = (t65 & t66);

  // depth 3  AND(t67, t64)
  assign t68 = (t67 & t64);

  // depth 1  AND(X1, X4)
  assign t69 = (X1 & X4);

  // depth 2  AND(t69, t0)
  assign t70 = (t69 & t0);

  // depth 1  AND(X0, t53)
  assign t71 = (X0 & t53);

  // depth 3  AND(t70, t71)
  assign t72 = (t70 & t71);

  // depth 1  NAND(X5, X7)
  assign t73 = ~(X5 & X7);

  // depth 1  AND(X3, X4)
  assign t74 = (X3 & X4);

  // depth 2  AND(t74, t4)
  assign t75 = (t74 & t4);

  // depth 3  AND(t75, t73)
  assign t76 = (t75 & t73);

  // depth 1  AND(X3, X4)
  assign t77 = (X3 & X4);

  // depth 2  AND(t77, X5)
  assign t78 = (t77 & X5);

  // depth 3  AND(t78, t56)
  assign t79 = (t78 & t56);

  // depth 1  NAND(X3, X4)
  assign t80 = ~(X3 & X4);

  // depth 1  AND(X0, X2)
  assign t81 = (X0 & X2);

  // depth 2  AND(t81, t80)
  assign t82 = (t81 & t80);

  // depth 3  AND(t82, t73)
  assign t83 = (t82 & t73);

  // depth 1  NAND(X0, X3)
  assign t84 = ~(X0 & X3);

  // depth 1  NAND(X1, X7)
  assign t85 = ~(X1 & X7);

  // depth 2  AND(t84, t85)
  assign t86 = (t84 & t85);

  // depth 2  AND(X5, t35)
  assign t87 = (X5 & t35);

  // depth 3  AND(t86, t87)
  assign t88 = (t86 & t87);

  // depth 1  NAND(X6, X7)
  assign t89 = ~(X6 & X7);

  // depth 1  AND(X0, X1)
  assign t90 = (X0 & X1);

  // depth 2  AND(t90, t14)
  assign t91 = (t90 & t14);

  // depth 3  AND(t91, t89)
  assign t92 = (t91 & t89);

  // depth 1  AND(X2, X3)
  assign t93 = (X2 & X3);

  // depth 2  NAND(t93, X7)
  assign t94 = ~(t93 & X7);

  // depth 1  AND(X1, X6)
  assign t95 = (X1 & X6);

  // depth 3  AND(t95, t94)
  assign t96 = (t95 & t94);

  // depth 1  NAND(X4, X6)
  assign t97 = ~(X4 & X6);

  // depth 1  AND(X1, X7)
  assign t98 = (X1 & X7);

  // depth 2  AND(t98, t40)
  assign t99 = (t98 & t40);

  // depth 3  AND(t99, t97)
  assign t100 = (t99 & t97);

  // depth 1  AND(X2, X6)
  assign t101 = (X2 & X6);

  // depth 2  NAND(t101, X7)
  assign t102 = ~(t101 & X7);

  // depth 1  AND(X4, t40)
  assign t103 = (X4 & t40);

  // depth 3  AND(t103, t102)
  assign t104 = (t103 & t102);

  // depth 1  AND(X3, X4)
  assign t105 = (X3 & X4);

  // depth 2  AND(t105, t27)
  assign t106 = (t105 & t27);

  // depth 1  AND(X2, t53)
  assign t107 = (X2 & t53);

  // depth 3  AND(t106, t107)
  assign t108 = (t106 & t107);

  // depth 1  AND(X1, X5)
  assign t109 = (X1 & X5);

  // depth 2  NAND(t109, X6)
  assign t110 = ~(t109 & X6);

  // depth 1  AND(X7, t40)
  assign t111 = (X7 & t40);

  // depth 3  AND(t111, t110)
  assign t112 = (t111 & t110);

  // depth 1  AND(X2, X6)
  assign t113 = (X2 & X6);

  // depth 1  AND(X0, X7)
  assign t114 = (X0 & X7);

  // depth 2  AND(t113, t114)
  assign t115 = (t113 & t114);

  // depth 3  AND(t115, t5)
  assign t116 = (t115 & t5);

  // depth 1  NAND(X1, X5)
  assign t117 = ~(X1 & X5);

  // depth 1  AND(X3, X6)
  assign t118 = (X3 & X6);

  // depth 2  AND(t118, t117)
  assign t119 = (t118 & t117);

  // depth 3  AND(t119, t56)
  assign t120 = (t119 & t56);

  // depth 1  AND(X1, X4)
  assign t121 = (X1 & X4);

  // depth 2  AND(t121, X5)
  assign t122 = (t121 & X5);

  // depth 3  AND(t122, t89)
  assign t123 = (t122 & t89);

  // depth 1  NAND(X4, X5)
  assign t124 = ~(X4 & X5);

  // depth 1  AND(X2, X6)
  assign t125 = (X2 & X6);

  // depth 2  AND(t125, t85)
  assign t126 = (t125 & t85);

  // depth 3  AND(t126, t124)
  assign t127 = (t126 & t124);

  // depth 1  AND(X0, X4)
  assign t128 = (X0 & X4);

  // depth 2  NAND(t128, X7)
  assign t129 = ~(t128 & X7);

  // depth 1  NAND(X3, X5)
  assign t130 = ~(X3 & X5);

  // depth 2  AND(X6, t130)
  assign t131 = (X6 & t130);

  // depth 3  AND(t129, t131)
  assign t132 = (t129 & t131);

  // depth 1  AND(X0, X4)
  assign t133 = (X0 & X4);

  // depth 2  NAND(t133, X5)
  assign t134 = ~(t133 & X5);

  // depth 1  AND(X6, X7)
  assign t135 = (X6 & X7);

  // depth 2  AND(t135, t10)
  assign t136 = (t135 & t10);

  // depth 3  AND(t134, t136)
  assign t137 = (t134 & t136);

  // depth 1  AND(X3, X6)
  assign t138 = (X3 & X6);

  // depth 1  AND(X2, t64)
  assign t139 = (X2 & t64);

  // depth 2  AND(t138, t139)
  assign t140 = (t138 & t139);

  // depth 3  AND(t140, t73)
  assign t141 = (t140 & t73);

  // depth 1  NAND(X0, X2)
  assign t142 = ~(X0 & X2);

  // depth 1  NAND(X3, X6)
  assign t143 = ~(X3 & X6);

  // depth 2  AND(t142, t143)
  assign t144 = (t142 & t143);

  // depth 1  AND(X4, X5)
  assign t145 = (X4 & X5);

  // depth 2  AND(t145, t53)
  assign t146 = (t145 & t53);

  // depth 3  AND(t144, t146)
  assign t147 = (t144 & t146);

  // depth 1  AND(X1, X3)
  assign t148 = (X1 & X3);

  // depth 1  AND(X6, X7)
  assign t149 = (X6 & X7);

  // depth 2  AND(X4, t149)
  assign t150 = (X4 & t149);

  // depth 3  AND(t148, t150)
  assign t151 = (t148 & t150);

  // depth 4  XOR2(t3, t9)
  assign t152 = (t3 ^ t9);

  // depth 4  XOR2(t13, t18)
  assign t153 = (t13 ^ t18);

  // depth 4  XOR2(t22, t26)
  assign t154 = (t22 ^ t26);

  // depth 4  XOR2(t30, t34)
  assign t155 = (t30 ^ t34);

  // depth 4  XOR2(t39, t45)
  assign t156 = (t39 ^ t45);

  // depth 4  XOR2(t52, t55)
  assign t157 = (t52 ^ t55);

  // depth 4  XOR2(t59, t63)
  assign t158 = (t59 ^ t63);

  // depth 4  XOR2(t68, t72)
  assign t159 = (t68 ^ t72);

  // depth 4  XOR2(t76, t79)
  assign t160 = (t76 ^ t79);

  // depth 4  XOR2(t83, t88)
  assign t161 = (t83 ^ t88);

  // depth 4  XOR2(t92, t96)
  assign t162 = (t92 ^ t96);

  // depth 4  XOR2(t100, t104)
  assign t163 = (t100 ^ t104);

  // depth 4  XOR2(t108, t112)
  assign t164 = (t108 ^ t112);

  // depth 4  XOR2(t116, t120)
  assign t165 = (t116 ^ t120);

  // depth 4  XOR2(t123, t127)
  assign t166 = (t123 ^ t127);

  // depth 4  XOR2(t132, t137)
  assign t167 = (t132 ^ t137);

  // depth 4  XOR2(t141, t147)
  assign t168 = (t141 ^ t147);

  // depth 5  XOR2(t152, t153)
  assign t169 = (t152 ^ t153);

  // depth 5  XOR2(t154, t155)
  assign t170 = (t154 ^ t155);

  // depth 5  XOR2(t156, t157)
  assign t171 = (t156 ^ t157);

  // depth 5  XOR2(t158, t159)
  assign t172 = (t158 ^ t159);

  // depth 5  XOR2(t160, t161)
  assign t173 = (t160 ^ t161);

  // depth 5  XOR2(t162, t163)
  assign t174 = (t162 ^ t163);

  // depth 5  XOR2(t164, t165)
  assign t175 = (t164 ^ t165);

  // depth 5  XOR2(t166, t167)
  assign t176 = (t166 ^ t167);

  // depth 5  XOR2(t168, t151)
  assign t177 = (t168 ^ t151);

  // depth 6  XOR2(t169, t170)
  assign t178 = (t169 ^ t170);

  // depth 6  XOR2(t171, t172)
  assign t179 = (t171 ^ t172);

  // depth 6  XOR2(t173, t174)
  assign t180 = (t173 ^ t174);

  // depth 6  XOR2(t175, t176)
  assign t181 = (t175 ^ t176);

  // depth 7  XOR2(t178, t179)
  assign t182 = (t178 ^ t179);

  // depth 7  XOR2(t180, t181)
  assign t183 = (t180 ^ t181);

  // depth 8  XOR2(t182, t183)
  assign t184 = (t182 ^ t183);

  // depth 9  XOR2(t184, t177)
  assign t185 = (t184 ^ t177);

  // depth 0  NOT(X3)
  assign t186 = ~X3;

  // depth 0  NOT(X4)
  assign t187 = ~X4;

  // depth 1  AND(X1, X2)
  assign t188 = (X1 & X2);

  // depth 1  AND(X0, t186)
  assign t189 = (X0 & t186);

  // depth 2  AND(t188, t189)
  assign t190 = (t188 & t189);

  // depth 3  AND(t190, t187)
  assign t191 = (t190 & t187);

  // depth 0  NOT(X0)
  assign t192 = ~X0;

  // depth 0  NOT(X1)
  assign t193 = ~X1;

  // depth 1  AND(X5, t192)
  assign t194 = (X5 & t192);

  // depth 1  AND(X3, t193)
  assign t195 = (X3 & t193);

  // depth 2  AND(t194, t195)
  assign t196 = (t194 & t195);

  // depth 1  AND(X2, t187)
  assign t197 = (X2 & t187);

  // depth 3  AND(t196, t197)
  assign t198 = (t196 & t197);

  // depth 1  AND(X2, X4)
  assign t199 = (X2 & X4);

  // depth 1  AND(X1, X6)
  assign t200 = (X1 & X6);

  // depth 2  AND(t199, t200)
  assign t201 = (t199 & t200);

  // depth 3  AND(t201, t192)
  assign t202 = (t201 & t192);

  // depth 1  AND(X1, X3)
  assign t203 = (X1 & X3);

  // depth 1  AND(X0, X6)
  assign t204 = (X0 & X6);

  // depth 2  AND(t203, t204)
  assign t205 = (t203 & t204);

  // depth 3  AND(t205, t187)
  assign t206 = (t205 & t187);

  // depth 0  NOT(X5)
  assign t207 = ~X5;

  // depth 1  AND(X2, X3)
  assign t208 = (X2 & X3);

  // depth 1  AND(X1, X6)
  assign t209 = (X1 & X6);

  // depth 2  AND(t208, t209)
  assign t210 = (t208 & t209);

  // depth 3  AND(t210, t207)
  assign t211 = (t210 & t207);

  // depth 1  NAND(X2, X6)
  assign t212 = ~(X2 & X6);

  // depth 1  AND(X1, X3)
  assign t213 = (X1 & X3);

  // depth 1  AND(X0, X5)
  assign t214 = (X0 & X5);

  // depth 2  AND(t213, t214)
  assign t215 = (t213 & t214);

  // depth 3  AND(t215, t212)
  assign t216 = (t215 & t212);

  // depth 1  AND(X1, X5)
  assign t217 = (X1 & X5);

  // depth 2  NAND(t217, X6)
  assign t218 = ~(t217 & X6);

  // depth 1  AND(X0, X2)
  assign t219 = (X0 & X2);

  // depth 2  AND(t219, t187)
  assign t220 = (t219 & t187);

  // depth 3  AND(t218, t220)
  assign t221 = (t218 & t220);

  // depth 1  NAND(X1, X3)
  assign t222 = ~(X1 & X3);

  // depth 1  AND(X4, X5)
  assign t223 = (X4 & X5);

  // depth 1  AND(X0, X6)
  assign t224 = (X0 & X6);

  // depth 2  AND(t223, t224)
  assign t225 = (t223 & t224);

  // depth 3  AND(t225, t222)
  assign t226 = (t225 & t222);

  // depth 1  NAND(X2, X4)
  assign t227 = ~(X2 & X4);

  // depth 1  AND(X3, X5)
  assign t228 = (X3 & X5);

  // depth 1  AND(X0, X6)
  assign t229 = (X0 & X6);

  // depth 2  AND(t228, t229)
  assign t230 = (t228 & t229);

  // depth 3  AND(t230, t227)
  assign t231 = (t230 & t227);

  // depth 1  NAND(X1, X4)
  assign t232 = ~(X1 & X4);

  // depth 1  AND(X5, X6)
  assign t233 = (X5 & X6);

  // depth 2  AND(t233, t232)
  assign t234 = (t233 & t232);

  // depth 1  AND(X2, t186)
  assign t235 = (X2 & t186);

  // depth 3  AND(t234, t235)
  assign t236 = (t234 & t235);

  // depth 1  NAND(X0, X1)
  assign t237 = ~(X0 & X1);

  // depth 1  AND(X2, X3)
  assign t238 = (X2 & X3);

  // depth 2  AND(t238, X7)
  assign t239 = (t238 & X7);

  // depth 3  AND(t239, t237)
  assign t240 = (t239 & t237);

  // depth 1  AND(X1, X2)
  assign t241 = (X1 & X2);

  // depth 1  AND(X0, X7)
  assign t242 = (X0 & X7);

  // depth 2  AND(t241, t242)
  assign t243 = (t241 & t242);

  // depth 3  AND(t243, t187)
  assign t244 = (t243 & t187);

  // depth 0  NOT(X2)
  assign t245 = ~X2;

  // depth 1  NAND(X3, X7)
  assign t246 = ~(X3 & X7);

  // depth 1  AND(X1, t245)
  assign t247 = (X1 & t245);

  // depth 2  AND(t247, t246)
  assign t248 = (t247 & t246);

  // depth 3  AND(t248, t187)
  assign t249 = (t248 & t187);

  // depth 1  AND(X5, X7)
  assign t250 = (X5 & X7);

  // depth 1  AND(X1, t192)
  assign t251 = (X1 & t192);

  // depth 2  AND(t250, t251)
  assign t252 = (t250 & t251);

  // depth 3  AND(t252, t245)
  assign t253 = (t252 & t245);

  // depth 1  AND(X1, X2)
  assign t254 = (X1 & X2);

  // depth 2  NAND(t254, X7)
  assign t255 = ~(t254 & X7);

  // depth 1  AND(X4, X5)
  assign t256 = (X4 & X5);

  // depth 2  AND(t256, t192)
  assign t257 = (t256 & t192);

  // depth 3  AND(t257, t255)
  assign t258 = (t257 & t255);

  // depth 1  NAND(X5, X7)
  assign t259 = ~(X5 & X7);

  // depth 1  AND(X0, X4)
  assign t260 = (X0 & X4);

  // depth 2  AND(t260, t222)
  assign t261 = (t260 & t222);

  // depth 3  AND(t261, t259)
  assign t262 = (t261 & t259);

  // depth 1  NAND(X2, X7)
  assign t263 = ~(X2 & X7);

  // depth 1  AND(X3, X4)
  assign t264 = (X3 & X4);

  // depth 2  AND(t264, X5)
  assign t265 = (t264 & X5);

  // depth 3  AND(t265, t263)
  assign t266 = (t265 & t263);

  // depth 1  AND(X0, X5)
  assign t267 = (X0 & X5);

  // depth 1  AND(X3, X7)
  assign t268 = (X3 & X7);

  // depth 2  AND(t267, t268)
  assign t269 = (t267 & t268);

  // depth 1  AND(X1, X2)
  assign t270 = (X1 & X2);

  // depth 2  NAND(t270, X4)
  assign t271 = ~(t270 & X4);

  // depth 3  AND(t269, t271)
  assign t272 = (t269 & t271);

  // depth 3  AND(t271, X3)
  assign t273 = (t271 & X3);

  // depth 1  NAND(X1, X6)
  assign t274 = ~(X1 & X6);

  // depth 1  AND(X0, X7)
  assign t275 = (X0 & X7);

  // depth 2  AND(t275, t274)
  assign t276 = (t275 & t274);

  // depth 0  NOT(X7)
  assign t277 = ~X7;

  // depth 1  AND(X2, X6)
  assign t278 = (X2 & X6);

  // depth 1  AND(X1, t192)
  assign t279 = (X1 & t192);

  // depth 2  AND(t278, t279)
  assign t280 = (t278 & t279);

  // depth 3  AND(t280, t277)
  assign t281 = (t280 & t277);

  // depth 1  NAND(X2, X3)
  assign t282 = ~(X2 & X3);

  // depth 1  AND(X0, X6)
  assign t283 = (X0 & X6);

  // depth 2  AND(t283, t282)
  assign t284 = (t283 & t282);

  // depth 3  AND(t284, t277)
  assign t285 = (t284 & t277);

  // depth 1  AND(X6, X7)
  assign t286 = (X6 & X7);

  // depth 1  AND(X1, t186)
  assign t287 = (X1 & t186);

  // depth 2  AND(t286, t287)
  assign t288 = (t286 & t287);

  // depth 3  AND(t288, t187)
  assign t289 = (t288 & t187);

  // depth 0  NOT(X6)
  assign t290 = ~X6;

  // depth 1  AND(X3, X4)
  assign t291 = (X3 & X4);

  // depth 2  AND(t291, t263)
  assign t292 = (t291 & t263);

  // depth 1  AND(X0, t290)
  assign t293 = (X0 & t290);

  // depth 3  AND(t292, t293)
  assign t294 = (t292 & t293);

  // depth 1  AND(X4, X7)
  assign t295 = (X4 & X7);

  // depth 2  AND(t295, t237)
  assign t296 = (t295 & t237);

  // depth 2  AND(X3, t212)
  assign t297 = (X3 & t212);

  // depth 3  AND(t296, t297)
  assign t298 = (t296 & t297);

  // depth 1  NAND(X0, X6)
  assign t299 = ~(X0 & X6);

  // depth 1  AND(X5, X7)
  assign t300 = (X5 & X7);

  // depth 2  AND(t300, t299)
  assign t301 = (t300 & t299);

  // depth 1  AND(X3, t193)
  assign t302 = (X3 & t193);

  // depth 3  AND(t301, t302)
  assign t303 = (t301 & t302);

  // depth 1  AND(X2, X5)
  assign t304 = (X2 & X5);

  // depth 2  AND(t304, t246)
  assign t305 = (t304 & t246);

  // depth 1  AND(X0, t290)
  assign t306 = (X0 & t290);

  // depth 3  AND(t305, t306)
  assign t307 = (t305 & t306);

  // depth 1  AND(X7, t207)
  assign t308 = (X7 & t207);

  // depth 1  AND(X4, t290)
  assign t309 = (X4 & t290);

  // depth 2  AND(t308, t309)
  assign t310 = (t308 & t309);

  // depth 1  NAND(X4, X7)
  assign t311 = ~(X4 & X7);

  // depth 1  NAND(X5, X6)
  assign t312 = ~(X5 & X6);

  // depth 1  AND(X0, t193)
  assign t313 = (X0 & t193);

  // depth 2  AND(t313, t311)
  assign t314 = (t313 & t311);

  // depth 3  AND(t314, t312)
  assign t315 = (t314 & t312);

  // depth 1  NAND(X0, X7)
  assign t316 = ~(X0 & X7);

  // depth 1  AND(X4, X5)
  assign t317 = (X4 & X5);

  // depth 2  AND(t317, t316)
  assign t318 = (t317 & t316);

  // depth 1  AND(X2, t290)
  assign t319 = (X2 & t290);

  // depth 3  AND(t318, t319)
  assign t320 = (t318 & t319);

  // depth 1  AND(X6, t193)
  assign t321 = (X6 & t193);

  // depth 2  AND(t321, t263)
  assign t322 = (t321 & t263);

  // depth 1  AND(X5, t187)
  assign t323 = (X5 & t187);

  // depth 3  AND(t322, t323)
  assign t324 = (t322 & t323);

  // depth 1  NAND(X0, X5)
  assign t325 = ~(X0 & X5);

  // depth 2  AND(t325, t232)
  assign t326 = (t325 & t232);

  // depth 2  AND(X7, t212)
  assign t327 = (X7 & t212);

  // depth 3  AND(t326, t327)
  assign t328 = (t326 & t327);

  // depth 1  NAND(X0, X4)
  assign t329 = ~(X0 & X4);

  // depth 1  AND(X6, X7)
  assign t330 = (X6 & X7);

  // depth 2  AND(t330, t329)
  assign t331 = (t330 & t329);

  // depth 1  AND(X3, t207)
  assign t332 = (X3 & t207);

  // depth 3  AND(t331, t332)
  assign t333 = (t331 & t332);

  // depth 1  AND(X4, X6)
  assign t334 = (X4 & X6);

  // depth 1  AND(X3, t193)
  assign t335 = (X3 & t193);

  // depth 2  AND(t334, t335)
  assign t336 = (t334 & t335);

  // depth 3  AND(t336, t259)
  assign t337 = (t336 & t259);

  // depth 1  AND(X1, X5)
  assign t338 = (X1 & X5);

  // depth 2  NAND(t338, X7)
  assign t339 = ~(t338 & X7);

  // depth 1  NAND(X4, X6)
  assign t340 = ~(X4 & X6);

  // depth 1  AND(X2, X3)
  assign t341 = (X2 & X3);

  // depth 2  AND(t341, t340)
  assign t342 = (t341 & t340);

  // depth 3  AND(t339, t342)
  assign t343 = (t339 & t342);

  // depth 4  XOR2(t191, t198)
  assign t344 = (t191 ^ t198);

  // depth 4  XOR2(t202, t206)
  assign t345 = (t202 ^ t206);

  // depth 4  XOR2(t211, t216)
  assign t346 = (t211 ^ t216);

  // depth 4  XOR2(t221, t226)
  assign t347 = (t221 ^ t226);

  // depth 4  XOR2(t231, t236)
  assign t348 = (t231 ^ t236);

  // depth 4  XOR2(t240, t244)
  assign t349 = (t240 ^ t244);

  // depth 4  XOR2(t249, t253)
  assign t350 = (t249 ^ t253);

  // depth 4  XOR2(t258, t262)
  assign t351 = (t258 ^ t262);

  // depth 4  XOR2(t266, t272)
  assign t352 = (t266 ^ t272);

  // depth 4  XOR2(t276, t281)
  assign t353 = (t276 ^ t281);

  // depth 4  XOR2(t285, t289)
  assign t354 = (t285 ^ t289);

  // depth 4  XOR2(t294, t298)
  assign t355 = (t294 ^ t298);

  // depth 4  XOR2(t303, t307)
  assign t356 = (t303 ^ t307);

  // depth 4  XOR2(t310, t315)
  assign t357 = (t310 ^ t315);

  // depth 4  XOR2(t320, t324)
  assign t358 = (t320 ^ t324);

  // depth 4  XOR2(t328, t333)
  assign t359 = (t328 ^ t333);

  // depth 4  XOR2(t337, t343)
  assign t360 = (t337 ^ t343);

  // depth 5  XOR2(t344, t345)
  assign t361 = (t344 ^ t345);

  // depth 5  XOR2(t346, t347)
  assign t362 = (t346 ^ t347);

  // depth 5  XOR2(t348, t349)
  assign t363 = (t348 ^ t349);

  // depth 5  XOR2(t350, t351)
  assign t364 = (t350 ^ t351);

  // depth 5  XOR2(t352, t353)
  assign t365 = (t352 ^ t353);

  // depth 5  XOR2(t354, t355)
  assign t366 = (t354 ^ t355);

  // depth 5  XOR2(t356, t357)
  assign t367 = (t356 ^ t357);

  // depth 5  XOR2(t358, t359)
  assign t368 = (t358 ^ t359);

  // depth 5  XOR2(t360, t273)
  assign t369 = (t360 ^ t273);

  // depth 6  XOR2(t361, t362)
  assign t370 = (t361 ^ t362);

  // depth 6  XOR2(t363, t364)
  assign t371 = (t363 ^ t364);

  // depth 6  XOR2(t365, t366)
  assign t372 = (t365 ^ t366);

  // depth 6  XOR2(t367, t368)
  assign t373 = (t367 ^ t368);

  // depth 7  XOR2(t370, t371)
  assign t374 = (t370 ^ t371);

  // depth 7  XOR2(t372, t373)
  assign t375 = (t372 ^ t373);

  // depth 8  XOR2(t374, t375)
  assign t376 = (t374 ^ t375);

  // depth 9  XOR2(t376, t369)
  assign t377 = (t376 ^ t369);

  // depth 0  NOT(X2)
  assign t378 = ~X2;

  // depth 1  AND(X1, X3)
  assign t379 = (X1 & X3);

  // depth 1  AND(X0, X5)
  assign t380 = (X0 & X5);

  // depth 2  AND(t379, t380)
  assign t381 = (t379 & t380);

  // depth 3  AND(t381, t378)
  assign t382 = (t381 & t378);

  // depth 0  NOT(X0)
  assign t383 = ~X0;

  // depth 1  AND(X3, X4)
  assign t384 = (X3 & X4);

  // depth 1  AND(X2, X5)
  assign t385 = (X2 & X5);

  // depth 2  AND(t384, t385)
  assign t386 = (t384 & t385);

  // depth 3  AND(t386, t383)
  assign t387 = (t386 & t383);

  // depth 1  NAND(X3, X6)
  assign t388 = ~(X3 & X6);

  // depth 2  AND(X4, t388)
  assign t389 = (X4 & t388);

  // depth 0  NOT(X3)
  assign t390 = ~X3;

  // depth 1  AND(X4, X6)
  assign t391 = (X4 & X6);

  // depth 1  AND(X2, t383)
  assign t392 = (X2 & t383);

  // depth 2  AND(t391, t392)
  assign t393 = (t391 & t392);

  // depth 3  AND(t393, t390)
  assign t394 = (t393 & t390);

  // depth 1  NAND(X1, X4)
  assign t395 = ~(X1 & X4);

  // depth 1  AND(X2, X3)
  assign t396 = (X2 & X3);

  // depth 1  AND(X0, X6)
  assign t397 = (X0 & X6);

  // depth 2  AND(t396, t397)
  assign t398 = (t396 & t397);

  // depth 3  AND(t398, t395)
  assign t399 = (t398 & t395);

  // depth 0  NOT(X6)
  assign t400 = ~X6;

  // depth 1  AND(X5, t378)
  assign t401 = (X5 & t378);

  // depth 1  AND(X4, t400)
  assign t402 = (X4 & t400);

  // depth 2  AND(t401, t402)
  assign t403 = (t401 & t402);

  // depth 0  NOT(X4)
  assign t404 = ~X4;

  // depth 0  NOT(X5)
  assign t405 = ~X5;

  // depth 1  AND(X2, X6)
  assign t406 = (X2 & X6);

  // depth 1  AND(X1, t404)
  assign t407 = (X1 & t404);

  // depth 2  AND(t406, t407)
  assign t408 = (t406 & t407);

  // depth 3  AND(t408, t405)
  assign t409 = (t408 & t405);

  // depth 1  NAND(X1, X5)
  assign t410 = ~(X1 & X5);

  // depth 1  AND(X3, X4)
  assign t411 = (X3 & X4);

  // depth 2  AND(t411, t410)
  assign t412 = (t411 & t410);

  // depth 1  AND(X0, t400)
  assign t413 = (X0 & t400);

  // depth 3  AND(t412, t413)
  assign t414 = (t412 & t413);

  // depth 1  NAND(X5, X6)
  assign t415 = ~(X5 & X6);

  // depth 1  AND(X3, X4)
  assign t416 = (X3 & X4);

  // depth 1  AND(X1, t378)
  assign t417 = (X1 & t378);

  // depth 2  AND(t416, t417)
  assign t418 = (t416 & t417);

  // depth 3  AND(t418, t415)
  assign t419 = (t418 & t415);

  // depth 0  NOT(X1)
  assign t420 = ~X1;

  // depth 1  AND(X3, X7)
  assign t421 = (X3 & X7);

  // depth 1  AND(X2, t383)
  assign t422 = (X2 & t383);

  // depth 2  AND(t421, t422)
  assign t423 = (t421 & t422);

  // depth 3  AND(t423, t420)
  assign t424 = (t423 & t420);

  // depth 0  NOT(X7)
  assign t425 = ~X7;

  // depth 1  AND(X2, X3)
  assign t426 = (X2 & X3);

  // depth 1  AND(X0, X4)
  assign t427 = (X0 & X4);

  // depth 2  AND(t426, t427)
  assign t428 = (t426 & t427);

  // depth 3  AND(t428, t425)
  assign t429 = (t428 & t425);

  // depth 1  AND(X4, X5)
  assign t430 = (X4 & X5);

  // depth 1  AND(X1, t425)
  assign t431 = (X1 & t425);

  // depth 2  AND(t430, t431)
  assign t432 = (t430 & t431);

  // depth 1  AND(X1, X5)
  assign t433 = (X1 & X5);

  // depth 2  NAND(t433, X7)
  assign t434 = ~(t433 & X7);

  // depth 1  AND(X0, X4)
  assign t435 = (X0 & X4);

  // depth 2  AND(t435, t378)
  assign t436 = (t435 & t378);

  // depth 3  AND(t434, t436)
  assign t437 = (t434 & t436);

  // depth 1  AND(X3, X4)
  assign t438 = (X3 & X4);

  // depth 2  NAND(t438, X5)
  assign t439 = ~(t438 & X5);

  // depth 1  AND(X0, X7)
  assign t440 = (X0 & X7);

  // depth 2  AND(t440, t420)
  assign t441 = (t440 & t420);

  // depth 3  AND(t441, t439)
  assign t442 = (t441 & t439);

  // depth 1  NAND(X0, X5)
  assign t443 = ~(X0 & X5);

  // depth 1  NAND(X4, X7)
  assign t444 = ~(X4 & X7);

  // depth 1  AND(X2, X3)
  assign t445 = (X2 & X3);

  // depth 2  AND(t445, t443)
  assign t446 = (t445 & t443);

  // depth 3  AND(t446, t444)
  assign t447 = (t446 & t444);

  // depth 1  NAND(X2, X5)
  assign t448 = ~(X2 & X5);

  // depth 1  AND(X7, t420)
  assign t449 = (X7 & t420);

  // depth 2  AND(t449, t448)
  assign t450 = (t449 & t448);

  // depth 1  AND(X3, t404)
  assign t451 = (X3 & t404);

  // depth 3  AND(t450, t451)
  assign t452 = (t450 & t451);

  // depth 1  NAND(X6, X7)
  assign t453 = ~(X6 & X7);

  // depth 1  AND(X0, X1)
  assign t454 = (X0 & X1);

  // depth 2  AND(t454, X2)
  assign t455 = (t454 & X2);

  // depth 3  AND(t455, t453)
  assign t456 = (t455 & t453);

  // depth 1  AND(X0, X2)
  assign t457 = (X0 & X2);

  // depth 2  NAND(t457, X7)
  assign t458 = ~(t457 & X7);

  // depth 1  NAND(X4, X6)
  assign t459 = ~(X4 & X6);

  // depth 3  AND(t458, t459)
  assign t460 = (t458 & t459);

  // depth 1  NAND(X3, X7)
  assign t461 = ~(X3 & X7);

  // depth 2  AND(X0, t461)
  assign t462 = (X0 & t461);

  // depth 3  AND(t462, t459)
  assign t463 = (t462 & t459);

  // depth 1  AND(X1, X4)
  assign t464 = (X1 & X4);

  // depth 1  AND(X0, t390)
  assign t465 = (X0 & t390);

  // depth 2  AND(t464, t465)
  assign t466 = (t464 & t465);

  // depth 3  AND(t466, t453)
  assign t467 = (t466 & t453);

  // depth 1  NAND(X1, X2)
  assign t468 = ~(X1 & X2);

  // depth 1  AND(X7, t390)
  assign t469 = (X7 & t390);

  // depth 2  AND(t468, t469)
  assign t470 = (t468 & t469);

  // depth 1  AND(X6, t404)
  assign t471 = (X6 & t404);

  // depth 3  AND(t470, t471)
  assign t472 = (t470 & t471);

  // depth 1  NAND(X0, X7)
  assign t473 = ~(X0 & X7);

  // depth 1  AND(X2, X4)
  assign t474 = (X2 & X4);

  // depth 2  AND(t474, t473)
  assign t475 = (t474 & t473);

  // depth 2  AND(X1, t388)
  assign t476 = (X1 & t388);

  // depth 3  AND(t475, t476)
  assign t477 = (t475 & t476);

  // depth 1  NAND(X1, X7)
  assign t478 = ~(X1 & X7);

  // depth 1  AND(X2, X5)
  assign t479 = (X2 & X5);

  // depth 2  AND(t479, t478)
  assign t480 = (t479 & t478);

  // depth 1  AND(X0, t400)
  assign t481 = (X0 & t400);

  // depth 3  AND(t480, t481)
  assign t482 = (t480 & t481);

  // depth 1  AND(X3, X6)
  assign t483 = (X3 & X6);

  // depth 1  AND(X0, t405)
  assign t484 = (X0 & t405);

  // depth 2  AND(t483, t484)
  assign t485 = (t483 & t484);

  // depth 3  AND(t485, t425)
  assign t486 = (t485 & t425);

  // depth 1  AND(X3, X6)
  assign t487 = (X3 & X6);

  // depth 2  NAND(t487, X7)
  assign t488 = ~(t487 & X7);

  // depth 1  AND(X1, X5)
  assign t489 = (X1 & X5);

  // depth 2  AND(t489, t383)
  assign t490 = (t489 & t383);

  // depth 3  AND(t490, t488)
  assign t491 = (t490 & t488);

  // depth 1  NAND(X2, X7)
  assign t492 = ~(X2 & X7);

  // depth 1  AND(X3, X6)
  assign t493 = (X3 & X6);

  // depth 2  AND(t493, t492)
  assign t494 = (t493 & t492);

  // depth 3  AND(t494, t405)
  assign t495 = (t494 & t405);

  // depth 1  AND(X2, X5)
  assign t496 = (X2 & X5);

  // depth 2  NAND(t496, X6)
  assign t497 = ~(t496 & X6);

  // depth 1  AND(X1, X3)
  assign t498 = (X1 & X3);

  // depth 2  AND(t498, t473)
  assign t499 = (t498 & t473);

  // depth 3  AND(t499, t497)
  assign t500 = (t499 & t497);

  // depth 1  NAND(X1, X6)
  assign t501 = ~(X1 & X6);

  // depth 1  AND(X7, t404)
  assign t502 = (X7 & t404);

  // depth 2  AND(t501, t502)
  assign t503 = (t501 & t502);

  // depth 3  AND(t503, t405)
  assign t504 = (t503 & t405);

  // depth 1  AND(X2, X5)
  assign t505 = (X2 & X5);

  // depth 2  NAND(t505, X7)
  assign t506 = ~(t505 & X7);

  // depth 2  AND(X1, t459)
  assign t507 = (X1 & t459);

  // depth 3  AND(t506, t507)
  assign t508 = (t506 & t507);

  // depth 1  NAND(X2, X4)
  assign t509 = ~(X2 & X4);

  // depth 2  AND(t501, t509)
  assign t510 = (t501 & t509);

  // depth 1  AND(X0, X5)
  assign t511 = (X0 & X5);

  // depth 2  AND(t511, t425)
  assign t512 = (t511 & t425);

  // depth 3  AND(t510, t512)
  assign t513 = (t510 & t512);

  // depth 1  AND(X4, X5)
  assign t514 = (X4 & X5);

  // depth 1  AND(X3, t383)
  assign t515 = (X3 & t383);

  // depth 2  AND(t514, t515)
  assign t516 = (t514 & t515);

  // depth 3  AND(t516, t453)
  assign t517 = (t516 & t453);

  // depth 1  AND(X4, X6)
  assign t518 = (X4 & X6);

  // depth 1  AND(X3, X7)
  assign t519 = (X3 & X7);

  // depth 2  AND(t518, t519)
  assign t520 = (t518 & t519);

  // depth 1  AND(X1, t405)
  assign t521 = (X1 & t405);

  // depth 3  AND(t520, t521)
  assign t522 = (t520 & t521);

  // depth 1  NAND(X1, X3)
  assign t523 = ~(X1 & X3);

  // depth 1  AND(X4, X5)
  assign t524 = (X4 & X5);

  // depth 2  NAND(t524, X7)
  assign t525 = ~(t524 & X7);

  // depth 1  AND(X0, X6)
  assign t526 = (X0 & X6);

  // depth 2  AND(t526, t523)
  assign t527 = (t526 & t523);

  // depth 3  AND(t527, t525)
  assign t528 = (t527 & t525);

  // depth 1  AND(X2, X3)
  assign t529 = (X2 & X3);

  // depth 2  AND(X0, t529)
  assign t530 = (X0 & t529);

  // depth 1  AND(X6, X7)
  assign t531 = (X6 & X7);

  // depth 2  AND(X5, t531)
  assign t532 = (X5 & t531);

  // depth 3  AND(t530, t532)
  assign t533 = (t530 & t532);

  // depth 4  XOR2(t382, t387)
  assign t534 = (t382 ^ t387);

  // depth 4  XOR2(t389, t394)
  assign t535 = (t389 ^ t394);

  // depth 4  XOR2(t399, t403)
  assign t536 = (t399 ^ t403);

  // depth 4  XOR2(t409, t414)
  assign t537 = (t409 ^ t414);

  // depth 4  XOR2(t419, t424)
  assign t538 = (t419 ^ t424);

  // depth 4  XOR2(t429, t432)
  assign t539 = (t429 ^ t432);

  // depth 4  XOR2(t437, t442)
  assign t540 = (t437 ^ t442);

  // depth 4  XOR2(t447, t452)
  assign t541 = (t447 ^ t452);

  // depth 4  XOR2(t456, t460)
  assign t542 = (t456 ^ t460);

  // depth 4  XOR2(t463, t467)
  assign t543 = (t463 ^ t467);

  // depth 4  XOR2(t472, t477)
  assign t544 = (t472 ^ t477);

  // depth 4  XOR2(t482, t486)
  assign t545 = (t482 ^ t486);

  // depth 4  XOR2(t491, t495)
  assign t546 = (t491 ^ t495);

  // depth 4  XOR2(t500, t504)
  assign t547 = (t500 ^ t504);

  // depth 4  XOR2(t508, t513)
  assign t548 = (t508 ^ t513);

  // depth 4  XOR2(t517, t522)
  assign t549 = (t517 ^ t522);

  // depth 4  XOR2(t528, t533)
  assign t550 = (t528 ^ t533);

  // depth 5  XOR2(t534, t535)
  assign t551 = (t534 ^ t535);

  // depth 5  XOR2(t536, t537)
  assign t552 = (t536 ^ t537);

  // depth 5  XOR2(t538, t539)
  assign t553 = (t538 ^ t539);

  // depth 5  XOR2(t540, t541)
  assign t554 = (t540 ^ t541);

  // depth 5  XOR2(t542, t543)
  assign t555 = (t542 ^ t543);

  // depth 5  XOR2(t544, t545)
  assign t556 = (t544 ^ t545);

  // depth 5  XOR2(t546, t547)
  assign t557 = (t546 ^ t547);

  // depth 5  XOR2(t548, t549)
  assign t558 = (t548 ^ t549);

  // depth 6  XOR2(t551, t552)
  assign t559 = (t551 ^ t552);

  // depth 6  XOR2(t553, t554)
  assign t560 = (t553 ^ t554);

  // depth 6  XOR2(t555, t556)
  assign t561 = (t555 ^ t556);

  // depth 6  XOR2(t557, t558)
  assign t562 = (t557 ^ t558);

  // depth 7  XOR2(t559, t560)
  assign t563 = (t559 ^ t560);

  // depth 7  XOR2(t561, t562)
  assign t564 = (t561 ^ t562);

  // depth 8  XOR2(t563, t564)
  assign t565 = (t563 ^ t564);

  // depth 9  XOR2(t565, t550)
  assign t566 = (t565 ^ t550);

  // depth 1  NAND(X1, X6)
  assign t567 = ~(X1 & X6);

  // depth 1  AND(X0, X2)
  assign t568 = (X0 & X2);

  // depth 2  AND(t568, X3)
  assign t569 = (t568 & X3);

  // depth 3  AND(t569, t567)
  assign t570 = (t569 & t567);

  // depth 0  NOT(X0)
  assign t571 = ~X0;

  // depth 1  AND(X4, X6)
  assign t572 = (X4 & X6);

  // depth 1  AND(X1, t571)
  assign t573 = (X1 & t571);

  // depth 2  AND(t572, t573)
  assign t574 = (t572 & t573);

  // depth 1  AND(X0, X2)
  assign t575 = (X0 & X2);

  // depth 2  AND(t575, X4)
  assign t576 = (t575 & X4);

  // depth 3  AND(t576, t567)
  assign t577 = (t576 & t567);

  // depth 1  NAND(X1, X2)
  assign t578 = ~(X1 & X2);

  // depth 1  NAND(X3, X4)
  assign t579 = ~(X3 & X4);

  // depth 1  AND(X0, X6)
  assign t580 = (X0 & X6);

  // depth 2  AND(t580, t578)
  assign t581 = (t580 & t578);

  // depth 3  AND(t581, t579)
  assign t582 = (t581 & t579);

  // depth 0  NOT(X6)
  assign t583 = ~X6;

  // depth 1  AND(X1, X2)
  assign t584 = (X1 & X2);

  // depth 1  AND(X0, X5)
  assign t585 = (X0 & X5);

  // depth 2  AND(t584, t585)
  assign t586 = (t584 & t585);

  // depth 3  AND(t586, t583)
  assign t587 = (t586 & t583);

  // depth 0  NOT(X5)
  assign t588 = ~X5;

  // depth 1  AND(X1, X3)
  assign t589 = (X1 & X3);

  // depth 1  AND(X0, t588)
  assign t590 = (X0 & t588);

  // depth 2  AND(t589, t590)
  assign t591 = (t589 & t590);

  // depth 3  AND(t591, t583)
  assign t592 = (t591 & t583);

  // depth 1  NAND(X3, X6)
  assign t593 = ~(X3 & X6);

  // depth 1  AND(X1, X2)
  assign t594 = (X1 & X2);

  // depth 2  AND(t594, X5)
  assign t595 = (t594 & X5);

  // depth 3  AND(t595, t593)
  assign t596 = (t595 & t593);

  // depth 0  NOT(X1)
  assign t597 = ~X1;

  // depth 1  AND(X4, X5)
  assign t598 = (X4 & X5);

  // depth 1  AND(X2, X6)
  assign t599 = (X2 & X6);

  // depth 2  AND(t598, t599)
  assign t600 = (t598 & t599);

  // depth 3  AND(t600, t597)
  assign t601 = (t600 & t597);

  // depth 0  NOT(X3)
  assign t602 = ~X3;

  // depth 1  AND(X5, X6)
  assign t603 = (X5 & X6);

  // depth 1  AND(X4, t602)
  assign t604 = (X4 & t602);

  // depth 2  AND(t603, t604)
  assign t605 = (t603 & t604);

  // depth 0  NOT(X2)
  assign t606 = ~X2;

  // depth 0  NOT(X4)
  assign t607 = ~X4;

  // depth 1  AND(X7, t606)
  assign t608 = (X7 & t606);

  // depth 1  AND(X1, t607)
  assign t609 = (X1 & t607);

  // depth 2  AND(t608, t609)
  assign t610 = (t608 & t609);

  // depth 1  AND(X1, X7)
  assign t611 = (X1 & X7);

  // depth 1  AND(X0, t602)
  assign t612 = (X0 & t602);

  // depth 2  AND(t611, t612)
  assign t613 = (t611 & t612);

  // depth 3  AND(t613, t607)
  assign t614 = (t613 & t607);

  // depth 1  NAND(X5, X7)
  assign t615 = ~(X5 & X7);

  // depth 1  AND(X1, X2)
  assign t616 = (X1 & X2);

  // depth 1  AND(X0, X3)
  assign t617 = (X0 & X3);

  // depth 2  AND(t616, t617)
  assign t618 = (t616 & t617);

  // depth 3  AND(t618, t615)
  assign t619 = (t618 & t615);

  // depth 1  AND(X2, X4)
  assign t620 = (X2 & X4);

  // depth 1  AND(X1, X7)
  assign t621 = (X1 & X7);

  // depth 2  AND(t620, t621)
  assign t622 = (t620 & t621);

  // depth 1  AND(X0, t588)
  assign t623 = (X0 & t588);

  // depth 3  AND(t622, t623)
  assign t624 = (t622 & t623);

  // depth 1  AND(X0, X7)
  assign t625 = (X0 & X7);

  // depth 2  AND(t625, t579)
  assign t626 = (t625 & t579);

  // depth 3  AND(t626, t588)
  assign t627 = (t626 & t588);

  // depth 1  NAND(X4, X7)
  assign t628 = ~(X4 & X7);

  // depth 1  AND(X1, X3)
  assign t629 = (X1 & X3);

  // depth 2  AND(t629, t628)
  assign t630 = (t629 & t628);

  // depth 3  AND(t630, t588)
  assign t631 = (t630 & t588);

  // depth 1  NAND(X0, X4)
  assign t632 = ~(X0 & X4);

  // depth 1  NAND(X1, X3)
  assign t633 = ~(X1 & X3);

  // depth 1  AND(X5, X7)
  assign t634 = (X5 & X7);

  // depth 2  AND(t634, t632)
  assign t635 = (t634 & t632);

  // depth 3  AND(t635, t633)
  assign t636 = (t635 & t633);

  // depth 1  AND(X0, X4)
  assign t637 = (X0 & X4);

  // depth 2  NAND(t637, X5)
  assign t638 = ~(t637 & X5);

  // depth 1  NAND(X2, X7)
  assign t639 = ~(X2 & X7);

  // depth 2  AND(X3, t639)
  assign t640 = (X3 & t639);

  // depth 3  AND(t638, t640)
  assign t641 = (t638 & t640);

  // depth 0  NOT(X7)
  assign t642 = ~X7;

  // depth 1  AND(X4, t588)
  assign t643 = (X4 & t588);

  // depth 2  AND(t633, t643)
  assign t644 = (t633 & t643);

  // depth 1  AND(X2, t642)
  assign t645 = (X2 & t642);

  // depth 3  AND(t644, t645)
  assign t646 = (t644 & t645);

  // depth 1  NAND(X0, X1)
  assign t647 = ~(X0 & X1);

  // depth 1  AND(X3, X4)
  assign t648 = (X3 & X4);

  // depth 2  AND(t648, t647)
  assign t649 = (t648 & t647);

  // depth 2  AND(X2, t615)
  assign t650 = (X2 & t615);

  // depth 3  AND(t649, t650)
  assign t651 = (t649 & t650);

  // depth 1  NAND(X6, X7)
  assign t652 = ~(X6 & X7);

  // depth 2  AND(X2, t633)
  assign t653 = (X2 & t633);

  // depth 3  AND(t653, t652)
  assign t654 = (t653 & t652);

  // depth 1  NAND(X0, X2)
  assign t655 = ~(X0 & X2);

  // depth 1  AND(X6, X7)
  assign t656 = (X6 & X7);

  // depth 2  AND(t656, t655)
  assign t657 = (t656 & t655);

  // depth 1  AND(X1, t607)
  assign t658 = (X1 & t607);

  // depth 3  AND(t657, t658)
  assign t659 = (t657 & t658);

  // depth 1  NAND(X3, X7)
  assign t660 = ~(X3 & X7);

  // depth 1  AND(X2, X4)
  assign t661 = (X2 & X4);

  // depth 2  AND(t661, X6)
  assign t662 = (t661 & X6);

  // depth 3  AND(t662, t660)
  assign t663 = (t662 & t660);

  // depth 1  AND(X0, t606)
  assign t664 = (X0 & t606);

  // depth 2  AND(t664, t593)
  assign t665 = (t664 & t593);

  // depth 3  AND(t665, t628)
  assign t666 = (t665 & t628);

  // depth 1  NAND(X1, X4)
  assign t667 = ~(X1 & X4);

  // depth 1  AND(X6, X7)
  assign t668 = (X6 & X7);

  // depth 2  AND(t668, t667)
  assign t669 = (t668 & t667);

  // depth 1  AND(X0, X3)
  assign t670 = (X0 & X3);

  // depth 2  AND(t670, t606)
  assign t671 = (t670 & t606);

  // depth 3  AND(t669, t671)
  assign t672 = (t669 & t671);

  // depth 1  AND(X0, X1)
  assign t673 = (X0 & X1);

  // depth 2  AND(t673, X5)
  assign t674 = (t673 & X5);

  // depth 3  AND(t674, t652)
  assign t675 = (t674 & t652);

  // depth 1  AND(X5, X7)
  assign t676 = (X5 & X7);

  // depth 1  AND(X2, t583)
  assign t677 = (X2 & t583);

  // depth 2  AND(t676, t677)
  assign t678 = (t676 & t677);

  // depth 1  AND(X5, X6)
  assign t679 = (X5 & X6);

  // depth 1  AND(X0, X7)
  assign t680 = (X0 & X7);

  // depth 2  AND(t679, t680)
  assign t681 = (t679 & t680);

  // depth 3  AND(t681, t606)
  assign t682 = (t681 & t606);

  // depth 1  AND(X3, X5)
  assign t683 = (X3 & X5);

  // depth 1  AND(X1, X6)
  assign t684 = (X1 & X6);

  // depth 2  AND(t683, t684)
  assign t685 = (t683 & t684);

  // depth 3  AND(t685, t642)
  assign t686 = (t685 & t642);

  // depth 1  NAND(X0, X7)
  assign t687 = ~(X0 & X7);

  // depth 1  NAND(X2, X6)
  assign t688 = ~(X2 & X6);

  // depth 1  AND(X3, X5)
  assign t689 = (X3 & X5);

  // depth 2  AND(t689, t687)
  assign t690 = (t689 & t687);

  // depth 3  AND(t690, t688)
  assign t691 = (t690 & t688);

  // depth 1  AND(X5, t597)
  assign t692 = (X5 & t597);

  // depth 2  AND(t692, t628)
  assign t693 = (t692 & t628);

  // depth 3  AND(t693, t583)
  assign t694 = (t693 & t583);

  // depth 1  NAND(X1, X7)
  assign t695 = ~(X1 & X7);

  // depth 1  AND(X4, X5)
  assign t696 = (X4 & X5);

  // depth 1  AND(X0, X6)
  assign t697 = (X0 & X6);

  // depth 2  AND(t696, t697)
  assign t698 = (t696 & t697);

  // depth 3  AND(t698, t695)
  assign t699 = (t698 & t695);

  // depth 1  AND(X6, X7)
  assign t700 = (X6 & X7);

  // depth 1  AND(X3, t607)
  assign t701 = (X3 & t607);

  // depth 2  AND(t700, t701)
  assign t702 = (t700 & t701);

  // depth 3  AND(t702, t588)
  assign t703 = (t702 & t588);

  // depth 4  XOR2(t570, t574)
  assign t704 = (t570 ^ t574);

  // depth 4  XOR2(t577, t582)
  assign t705 = (t577 ^ t582);

  // depth 4  XOR2(t587, t592)
  assign t706 = (t587 ^ t592);

  // depth 4  XOR2(t596, t601)
  assign t707 = (t596 ^ t601);

  // depth 3  XOR2(t605, t610)
  assign t708 = (t605 ^ t610);

  // depth 4  XOR2(t614, t619)
  assign t709 = (t614 ^ t619);

  // depth 4  XOR2(t624, t627)
  assign t710 = (t624 ^ t627);

  // depth 4  XOR2(t631, t636)
  assign t711 = (t631 ^ t636);

  // depth 4  XOR2(t641, t646)
  assign t712 = (t641 ^ t646);

  // depth 4  XOR2(t651, t654)
  assign t713 = (t651 ^ t654);

  // depth 4  XOR2(t659, t663)
  assign t714 = (t659 ^ t663);

  // depth 4  XOR2(t666, t672)
  assign t715 = (t666 ^ t672);

  // depth 4  XOR2(t675, t678)
  assign t716 = (t675 ^ t678);

  // depth 4  XOR2(t682, t686)
  assign t717 = (t682 ^ t686);

  // depth 4  XOR2(t691, t694)
  assign t718 = (t691 ^ t694);

  // depth 4  XOR2(t699, t703)
  assign t719 = (t699 ^ t703);

  // depth 5  XOR2(t704, t705)
  assign t720 = (t704 ^ t705);

  // depth 5  XOR2(t706, t707)
  assign t721 = (t706 ^ t707);

  // depth 5  XOR2(t708, t709)
  assign t722 = (t708 ^ t709);

  // depth 5  XOR2(t710, t711)
  assign t723 = (t710 ^ t711);

  // depth 5  XOR2(t712, t713)
  assign t724 = (t712 ^ t713);

  // depth 5  XOR2(t714, t715)
  assign t725 = (t714 ^ t715);

  // depth 5  XOR2(t716, t717)
  assign t726 = (t716 ^ t717);

  // depth 5  XOR2(t718, t719)
  assign t727 = (t718 ^ t719);

  // depth 6  XOR2(t720, t721)
  assign t728 = (t720 ^ t721);

  // depth 6  XOR2(t722, t723)
  assign t729 = (t722 ^ t723);

  // depth 6  XOR2(t724, t725)
  assign t730 = (t724 ^ t725);

  // depth 6  XOR2(t726, t727)
  assign t731 = (t726 ^ t727);

  // depth 7  XOR2(t728, t729)
  assign t732 = (t728 ^ t729);

  // depth 7  XOR2(t730, t731)
  assign t733 = (t730 ^ t731);

  // depth 8  XOR2(t732, t733)
  assign t734 = (t732 ^ t733);

  // depth 0  NOT(X0)
  assign t735 = ~X0;

  // depth 1  AND(X2, X3)
  assign t736 = (X2 & X3);

  // depth 1  AND(X1, t735)
  assign t737 = (X1 & t735);

  // depth 2  AND(t736, t737)
  assign t738 = (t736 & t737);

  // depth 1  NAND(X1, X3)
  assign t739 = ~(X1 & X3);

  // depth 1  AND(X0, X2)
  assign t740 = (X0 & X2);

  // depth 2  AND(t740, X4)
  assign t741 = (t740 & X4);

  // depth 3  AND(t741, t739)
  assign t742 = (t741 & t739);

  // depth 0  NOT(X1)
  assign t743 = ~X1;

  // depth 1  AND(X2, X4)
  assign t744 = (X2 & X4);

  // depth 1  AND(X0, X5)
  assign t745 = (X0 & X5);

  // depth 2  AND(t744, t745)
  assign t746 = (t744 & t745);

  // depth 3  AND(t746, t743)
  assign t747 = (t746 & t743);

  // depth 1  AND(X4, X5)
  assign t748 = (X4 & X5);

  // depth 2  AND(t748, t735)
  assign t749 = (t748 & t735);

  // depth 3  AND(t749, t739)
  assign t750 = (t749 & t739);

  // depth 1  AND(X3, X6)
  assign t751 = (X3 & X6);

  // depth 1  AND(X1, t735)
  assign t752 = (X1 & t735);

  // depth 2  AND(t751, t752)
  assign t753 = (t751 & t752);

  // depth 1  NAND(X1, X2)
  assign t754 = ~(X1 & X2);

  // depth 0  NOT(X4)
  assign t755 = ~X4;

  // depth 1  AND(X3, X6)
  assign t756 = (X3 & X6);

  // depth 2  AND(t756, t754)
  assign t757 = (t756 & t754);

  // depth 1  AND(X0, t755)
  assign t758 = (X0 & t755);

  // depth 3  AND(t757, t758)
  assign t759 = (t757 & t758);

  // depth 1  NAND(X2, X3)
  assign t760 = ~(X2 & X3);

  // depth 1  AND(X1, X5)
  assign t761 = (X1 & X5);

  // depth 2  AND(t761, X6)
  assign t762 = (t761 & X6);

  // depth 3  AND(t762, t760)
  assign t763 = (t762 & t760);

  // depth 1  NAND(X2, X4)
  assign t764 = ~(X2 & X4);

  // depth 0  NOT(X5)
  assign t765 = ~X5;

  // depth 1  AND(X0, X6)
  assign t766 = (X0 & X6);

  // depth 2  AND(t766, t764)
  assign t767 = (t766 & t764);

  // depth 3  AND(t767, t765)
  assign t768 = (t767 & t765);

  // depth 0  NOT(X2)
  assign t769 = ~X2;

  // depth 0  NOT(X6)
  assign t770 = ~X6;

  // depth 1  AND(X4, X5)
  assign t771 = (X4 & X5);

  // depth 1  AND(X3, t769)
  assign t772 = (X3 & t769);

  // depth 2  AND(t771, t772)
  assign t773 = (t771 & t772);

  // depth 3  AND(t773, t770)
  assign t774 = (t773 & t770);

  // depth 1  AND(X0, X7)
  assign t775 = (X0 & X7);

  // depth 2  AND(t775, t739)
  assign t776 = (t775 & t739);

  // depth 3  AND(t776, t769)
  assign t777 = (t776 & t769);

  // depth 1  AND(X0, X3)
  assign t778 = (X0 & X3);

  // depth 2  NAND(t778, X7)
  assign t779 = ~(t778 & X7);

  // depth 3  AND(t779, t755)
  assign t780 = (t779 & t755);

  // depth 1  NAND(X5, X7)
  assign t781 = ~(X5 & X7);

  // depth 1  AND(X0, X2)
  assign t782 = (X0 & X2);

  // depth 2  AND(t782, t743)
  assign t783 = (t782 & t743);

  // depth 3  AND(t783, t781)
  assign t784 = (t783 & t781);

  // depth 0  NOT(X7)
  assign t785 = ~X7;

  // depth 1  AND(X3, t765)
  assign t786 = (X3 & t765);

  // depth 1  AND(X1, t785)
  assign t787 = (X1 & t785);

  // depth 2  AND(t786, t787)
  assign t788 = (t786 & t787);

  // depth 1  NAND(X1, X5)
  assign t789 = ~(X1 & X5);

  // depth 1  AND(X2, X4)
  assign t790 = (X2 & X4);

  // depth 2  AND(t790, t789)
  assign t791 = (t790 & t789);

  // depth 3  AND(t791, t785)
  assign t792 = (t791 & t785);

  // depth 0  NOT(X3)
  assign t793 = ~X3;

  // depth 1  AND(X5, X7)
  assign t794 = (X5 & X7);

  // depth 1  AND(X2, t793)
  assign t795 = (X2 & t793);

  // depth 2  AND(t794, t795)
  assign t796 = (t794 & t795);

  // depth 3  AND(t796, t755)
  assign t797 = (t796 & t755);

  // depth 1  NAND(X6, X7)
  assign t798 = ~(X6 & X7);

  // depth 1  AND(X0, X4)
  assign t799 = (X0 & X4);

  // depth 2  AND(t799, t743)
  assign t800 = (t799 & t743);

  // depth 3  AND(t800, t798)
  assign t801 = (t800 & t798);

  // depth 1  NAND(X0, X7)
  assign t802 = ~(X0 & X7);

  // depth 1  AND(X3, X4)
  assign t803 = (X3 & X4);

  // depth 2  AND(t803, t802)
  assign t804 = (t803 & t802);

  // depth 1  AND(X1, t770)
  assign t805 = (X1 & t770);

  // depth 3  AND(t804, t805)
  assign t806 = (t804 & t805);

  // depth 1  NAND(X4, X6)
  assign t807 = ~(X4 & X6);

  // depth 1  AND(X2, X3)
  assign t808 = (X2 & X3);

  // depth 2  AND(t808, X7)
  assign t809 = (t808 & X7);

  // depth 3  AND(t809, t807)
  assign t810 = (t809 & t807);

  // depth 1  AND(X3, X6)
  assign t811 = (X3 & X6);

  // depth 1  AND(X2, t755)
  assign t812 = (X2 & t755);

  // depth 2  AND(t811, t812)
  assign t813 = (t811 & t812);

  // depth 1  AND(X0, t785)
  assign t814 = (X0 & t785);

  // depth 3  AND(t813, t814)
  assign t815 = (t813 & t814);

  // depth 1  AND(X1, X2)
  assign t816 = (X1 & X2);

  // depth 2  NAND(t816, X3)
  assign t817 = ~(t816 & X3);

  // depth 1  AND(X4, X6)
  assign t818 = (X4 & X6);

  // depth 2  AND(t818, t785)
  assign t819 = (t818 & t785);

  // depth 3  AND(t817, t819)
  assign t820 = (t817 & t819);

  // depth 1  AND(X0, X2)
  assign t821 = (X0 & X2);

  // depth 2  NAND(t821, X6)
  assign t822 = ~(t821 & X6);

  // depth 1  AND(X4, X7)
  assign t823 = (X4 & X7);

  // depth 1  AND(X1, t793)
  assign t824 = (X1 & t793);

  // depth 2  AND(t823, t824)
  assign t825 = (t823 & t824);

  // depth 3  AND(t822, t825)
  assign t826 = (t822 & t825);

  // depth 1  NAND(X2, X5)
  assign t827 = ~(X2 & X5);

  // depth 1  AND(X1, X6)
  assign t828 = (X1 & X6);

  // depth 2  AND(t828, t827)
  assign t829 = (t828 & t827);

  // depth 1  AND(X0, t785)
  assign t830 = (X0 & t785);

  // depth 3  AND(t829, t830)
  assign t831 = (t829 & t830);

  // depth 1  AND(X7, t765)
  assign t832 = (X7 & t765);

  // depth 1  AND(X3, t770)
  assign t833 = (X3 & t770);

  // depth 2  AND(t832, t833)
  assign t834 = (t832 & t833);

  // depth 1  NAND(X1, X7)
  assign t835 = ~(X1 & X7);

  // depth 1  AND(X3, X5)
  assign t836 = (X3 & X5);

  // depth 2  AND(t836, t835)
  assign t837 = (t836 & t835);

  // depth 1  AND(X0, t770)
  assign t838 = (X0 & t770);

  // depth 3  AND(t837, t838)
  assign t839 = (t837 & t838);

  // depth 1  AND(X0, X3)
  assign t840 = (X0 & X3);

  // depth 2  NAND(t840, X5)
  assign t841 = ~(t840 & X5);

  // depth 1  AND(X2, X7)
  assign t842 = (X2 & X7);

  // depth 2  AND(t842, t770)
  assign t843 = (t842 & t770);

  // depth 3  AND(t841, t843)
  assign t844 = (t841 & t843);

  // depth 1  AND(X3, X5)
  assign t845 = (X3 & X5);

  // depth 2  NAND(t845, X6)
  assign t846 = ~(t845 & X6);

  // depth 2  AND(X2, t835)
  assign t847 = (X2 & t835);

  // depth 3  AND(t847, t846)
  assign t848 = (t847 & t846);

  // depth 1  AND(X0, X2)
  assign t849 = (X0 & X2);

  // depth 1  AND(X3, X6)
  assign t850 = (X3 & X6);

  // depth 2  NAND(t849, t850)
  assign t851 = ~(t849 & t850);

  // depth 1  AND(X1, X5)
  assign t852 = (X1 & X5);

  // depth 2  AND(t852, X7)
  assign t853 = (t852 & X7);

  // depth 3  AND(t853, t851)
  assign t854 = (t853 & t851);

  // depth 1  AND(X4, X5)
  assign t855 = (X4 & X5);

  // depth 1  AND(X0, X7)
  assign t856 = (X0 & X7);

  // depth 2  AND(t855, t856)
  assign t857 = (t855 & t856);

  // depth 3  AND(t857, t770)
  assign t858 = (t857 & t770);

  // depth 1  AND(X4, X6)
  assign t859 = (X4 & X6);

  // depth 1  AND(X1, t765)
  assign t860 = (X1 & t765);

  // depth 2  AND(t859, t860)
  assign t861 = (t859 & t860);

  // depth 3  AND(t861, t785)
  assign t862 = (t861 & t785);

  // depth 1  AND(X5, X6)
  assign t863 = (X5 & X6);

  // depth 1  AND(X2, t755)
  assign t864 = (X2 & t755);

  // depth 2  AND(t863, t864)
  assign t865 = (t863 & t864);

  // depth 3  AND(t865, t785)
  assign t866 = (t865 & t785);

  // depth 1  NAND(X2, X6)
  assign t867 = ~(X2 & X6);

  // depth 1  AND(X4, X5)
  assign t868 = (X4 & X5);

  // depth 2  NAND(t868, X7)
  assign t869 = ~(t868 & X7);

  // depth 2  AND(X1, t867)
  assign t870 = (X1 & t867);

  // depth 3  AND(t870, t869)
  assign t871 = (t870 & t869);

  // depth 1  NAND(X0, X5)
  assign t872 = ~(X0 & X5);

  // depth 1  AND(X4, X6)
  assign t873 = (X4 & X6);

  // depth 2  AND(t873, t872)
  assign t874 = (t873 & t872);

  // depth 1  AND(X3, t785)
  assign t875 = (X3 & t785);

  // depth 3  AND(t874, t875)
  assign t876 = (t874 & t875);

  // depth 4  XOR2(t738, t742)
  assign t877 = (t738 ^ t742);

  // depth 4  XOR2(t747, t750)
  assign t878 = (t747 ^ t750);

  // depth 4  XOR2(t753, t759)
  assign t879 = (t753 ^ t759);

  // depth 4  XOR2(t763, t768)
  assign t880 = (t763 ^ t768);

  // depth 4  XOR2(t774, t777)
  assign t881 = (t774 ^ t777);

  // depth 4  XOR2(t780, t784)
  assign t882 = (t780 ^ t784);

  // depth 4  XOR2(t788, t792)
  assign t883 = (t788 ^ t792);

  // depth 4  XOR2(t797, t801)
  assign t884 = (t797 ^ t801);

  // depth 4  XOR2(t806, t810)
  assign t885 = (t806 ^ t810);

  // depth 4  XOR2(t815, t820)
  assign t886 = (t815 ^ t820);

  // depth 4  XOR2(t826, t831)
  assign t887 = (t826 ^ t831);

  // depth 4  XOR2(t834, t839)
  assign t888 = (t834 ^ t839);

  // depth 4  XOR2(t844, t848)
  assign t889 = (t844 ^ t848);

  // depth 4  XOR2(t854, t858)
  assign t890 = (t854 ^ t858);

  // depth 4  XOR2(t862, t866)
  assign t891 = (t862 ^ t866);

  // depth 4  XOR2(t871, t876)
  assign t892 = (t871 ^ t876);

  // depth 5  XOR2(t877, t878)
  assign t893 = (t877 ^ t878);

  // depth 5  XOR2(t879, t880)
  assign t894 = (t879 ^ t880);

  // depth 5  XOR2(t881, t882)
  assign t895 = (t881 ^ t882);

  // depth 5  XOR2(t883, t884)
  assign t896 = (t883 ^ t884);

  // depth 5  XOR2(t885, t886)
  assign t897 = (t885 ^ t886);

  // depth 5  XOR2(t887, t888)
  assign t898 = (t887 ^ t888);

  // depth 5  XOR2(t889, t890)
  assign t899 = (t889 ^ t890);

  // depth 5  XOR2(t891, t892)
  assign t900 = (t891 ^ t892);

  // depth 6  XOR2(t893, t894)
  assign t901 = (t893 ^ t894);

  // depth 6  XOR2(t895, t896)
  assign t902 = (t895 ^ t896);

  // depth 6  XOR2(t897, t898)
  assign t903 = (t897 ^ t898);

  // depth 6  XOR2(t899, t900)
  assign t904 = (t899 ^ t900);

  // depth 7  XOR2(t901, t902)
  assign t905 = (t901 ^ t902);

  // depth 7  XOR2(t903, t904)
  assign t906 = (t903 ^ t904);

  // depth 8  XOR2(t905, t906)
  assign t907 = (t905 ^ t906);

  // depth 0  NOT(X3)
  assign t908 = ~X3;

  // depth 0  NOT(X4)
  assign t909 = ~X4;

  // depth 1  AND(X2, t908)
  assign t910 = (X2 & t908);

  // depth 1  AND(X0, t909)
  assign t911 = (X0 & t909);

  // depth 2  AND(t910, t911)
  assign t912 = (t910 & t911);

  // depth 0  NOT(X1)
  assign t913 = ~X1;

  // depth 1  NAND(X2, X4)
  assign t914 = ~(X2 & X4);

  // depth 1  AND(X3, t913)
  assign t915 = (X3 & t913);

  // depth 2  AND(t915, t914)
  assign t916 = (t915 & t914);

  // depth 0  NOT(X0)
  assign t917 = ~X0;

  // depth 0  NOT(X2)
  assign t918 = ~X2;

  // depth 1  AND(X5, t917)
  assign t919 = (X5 & t917);

  // depth 1  AND(X3, t918)
  assign t920 = (X3 & t918);

  // depth 2  AND(t919, t920)
  assign t921 = (t919 & t920);

  // depth 1  AND(X4, X5)
  assign t922 = (X4 & X5);

  // depth 1  AND(X1, t918)
  assign t923 = (X1 & t918);

  // depth 2  AND(t922, t923)
  assign t924 = (t922 & t923);

  // depth 3  AND(t924, t908)
  assign t925 = (t924 & t908);

  // depth 1  AND(X4, X6)
  assign t926 = (X4 & X6);

  // depth 1  AND(X1, t918)
  assign t927 = (X1 & t918);

  // depth 2  AND(t926, t927)
  assign t928 = (t926 & t927);

  // depth 3  AND(t928, t908)
  assign t929 = (t928 & t908);

  // depth 1  AND(X0, X2)
  assign t930 = (X0 & X2);

  // depth 2  NAND(t930, X5)
  assign t931 = ~(t930 & X5);

  // depth 1  AND(X1, X3)
  assign t932 = (X1 & X3);

  // depth 2  NAND(t932, X6)
  assign t933 = ~(t932 & X6);

  // depth 3  AND(t931, t933)
  assign t934 = (t931 & t933);

  // depth 1  AND(X5, X6)
  assign t935 = (X5 & X6);

  // depth 1  AND(X2, t909)
  assign t936 = (X2 & t909);

  // depth 2  AND(t935, t936)
  assign t937 = (t935 & t936);

  // depth 1  AND(X2, X4)
  assign t938 = (X2 & X4);

  // depth 2  NAND(t938, X5)
  assign t939 = ~(t938 & X5);

  // depth 1  AND(X0, X1)
  assign t940 = (X0 & X1);

  // depth 2  AND(t940, X6)
  assign t941 = (t940 & X6);

  // depth 3  AND(t941, t939)
  assign t942 = (t941 & t939);

  // depth 0  NOT(X5)
  assign t943 = ~X5;

  // depth 1  AND(X3, X6)
  assign t944 = (X3 & X6);

  // depth 1  AND(X0, t909)
  assign t945 = (X0 & t909);

  // depth 2  AND(t944, t945)
  assign t946 = (t944 & t945);

  // depth 3  AND(t946, t943)
  assign t947 = (t946 & t943);

  // depth 1  AND(X4, X5)
  assign t948 = (X4 & X5);

  // depth 1  AND(X1, X6)
  assign t949 = (X1 & X6);

  // depth 2  AND(t948, t949)
  assign t950 = (t948 & t949);

  // depth 3  AND(t950, t908)
  assign t951 = (t950 & t908);

  // depth 1  AND(X0, X3)
  assign t952 = (X0 & X3);

  // depth 2  NAND(t952, X4)
  assign t953 = ~(t952 & X4);

  // depth 1  AND(X1, X6)
  assign t954 = (X1 & X6);

  // depth 2  AND(t954, t943)
  assign t955 = (t954 & t943);

  // depth 3  AND(t953, t955)
  assign t956 = (t953 & t955);

  // depth 1  AND(X1, X2)
  assign t957 = (X1 & X2);

  // depth 1  AND(X0, X7)
  assign t958 = (X0 & X7);

  // depth 2  AND(t957, t958)
  assign t959 = (t957 & t958);

  // depth 3  AND(t959, t908)
  assign t960 = (t959 & t908);

  // depth 1  NAND(X0, X3)
  assign t961 = ~(X0 & X3);

  // depth 1  AND(X4, X7)
  assign t962 = (X4 & X7);

  // depth 2  AND(t962, t961)
  assign t963 = (t962 & t961);

  // depth 3  AND(t963, t918)
  assign t964 = (t963 & t918);

  // depth 1  NAND(X3, X4)
  assign t965 = ~(X3 & X4);

  // depth 1  AND(X1, X2)
  assign t966 = (X1 & X2);

  // depth 2  AND(t966, X7)
  assign t967 = (t966 & X7);

  // depth 3  AND(t967, t965)
  assign t968 = (t967 & t965);

  // depth 1  AND(X5, X7)
  assign t969 = (X5 & X7);

  // depth 1  AND(X3, t918)
  assign t970 = (X3 & t918);

  // depth 2  AND(t969, t970)
  assign t971 = (t969 & t970);

  // depth 1  NAND(X0, X4)
  assign t972 = ~(X0 & X4);

  // depth 1  AND(X1, X5)
  assign t973 = (X1 & X5);

  // depth 2  AND(t973, X7)
  assign t974 = (t973 & X7);

  // depth 3  AND(t974, t972)
  assign t975 = (t974 & t972);

  // depth 0  NOT(X7)
  assign t976 = ~X7;

  // depth 1  AND(X3, X4)
  assign t977 = (X3 & X4);

  // depth 1  AND(X0, X5)
  assign t978 = (X0 & X5);

  // depth 2  AND(t977, t978)
  assign t979 = (t977 & t978);

  // depth 3  AND(t979, t976)
  assign t980 = (t979 & t976);

  // depth 1  NAND(X0, X1)
  assign t981 = ~(X0 & X1);

  // depth 1  NAND(X5, X7)
  assign t982 = ~(X5 & X7);

  // depth 1  AND(X3, X4)
  assign t983 = (X3 & X4);

  // depth 2  AND(t983, t981)
  assign t984 = (t983 & t981);

  // depth 3  AND(t984, t982)
  assign t985 = (t984 & t982);

  // depth 1  AND(X2, X6)
  assign t986 = (X2 & X6);

  // depth 1  AND(X0, t908)
  assign t987 = (X0 & t908);

  // depth 2  AND(t986, t987)
  assign t988 = (t986 & t987);

  // depth 3  AND(t988, t976)
  assign t989 = (t988 & t976);

  // depth 1  AND(X6, X7)
  assign t990 = (X6 & X7);

  // depth 1  AND(X3, t913)
  assign t991 = (X3 & t913);

  // depth 2  AND(t990, t991)
  assign t992 = (t990 & t991);

  // depth 3  AND(t992, t918)
  assign t993 = (t992 & t918);

  // depth 1  AND(X4, X6)
  assign t994 = (X4 & X6);

  // depth 1  AND(X0, t918)
  assign t995 = (X0 & t918);

  // depth 2  AND(t994, t995)
  assign t996 = (t994 & t995);

  // depth 3  AND(t996, t976)
  assign t997 = (t996 & t976);

  // depth 1  NAND(X0, X5)
  assign t998 = ~(X0 & X5);

  // depth 1  NAND(X6, X7)
  assign t999 = ~(X6 & X7);

  // depth 2  AND(X1, t998)
  assign t1000 = (X1 & t998);

  // depth 3  AND(t1000, t999)
  assign t1001 = (t1000 & t999);

  // depth 1  NAND(X1, X7)
  assign t1002 = ~(X1 & X7);

  // depth 1  AND(X3, X5)
  assign t1003 = (X3 & X5);

  // depth 2  AND(t1003, X6)
  assign t1004 = (t1003 & X6);

  // depth 3  AND(t1004, t1002)
  assign t1005 = (t1004 & t1002);

  // depth 1  NAND(X3, X5)
  assign t1006 = ~(X3 & X5);

  // depth 1  AND(X6, t913)
  assign t1007 = (X6 & t913);

  // depth 2  AND(t1007, t1006)
  assign t1008 = (t1007 & t1006);

  // depth 1  AND(X2, t976)
  assign t1009 = (X2 & t976);

  // depth 3  AND(t1008, t1009)
  assign t1010 = (t1008 & t1009);

  // depth 1  NAND(X2, X5)
  assign t1011 = ~(X2 & X5);

  // depth 0  NOT(X6)
  assign t1012 = ~X6;

  // depth 1  AND(X7, t913)
  assign t1013 = (X7 & t913);

  // depth 2  AND(t1013, t1011)
  assign t1014 = (t1013 & t1011);

  // depth 1  AND(X0, X3)
  assign t1015 = (X0 & X3);

  // depth 2  AND(t1015, t1012)
  assign t1016 = (t1015 & t1012);

  // depth 3  AND(t1014, t1016)
  assign t1017 = (t1014 & t1016);

  // depth 1  AND(X5, X7)
  assign t1018 = (X5 & X7);

  // depth 1  AND(X4, t917)
  assign t1019 = (X4 & t917);

  // depth 2  AND(t1018, t1019)
  assign t1020 = (t1018 & t1019);

  // depth 1  AND(X2, t1012)
  assign t1021 = (X2 & t1012);

  // depth 3  AND(t1020, t1021)
  assign t1022 = (t1020 & t1021);

  // depth 1  NAND(X1, X4)
  assign t1023 = ~(X1 & X4);

  // depth 1  AND(X6, X7)
  assign t1024 = (X6 & X7);

  // depth 2  AND(t1024, t1023)
  assign t1025 = (t1024 & t1023);

  // depth 1  AND(X5, t918)
  assign t1026 = (X5 & t918);

  // depth 3  AND(t1025, t1026)
  assign t1027 = (t1025 & t1026);

  // depth 1  NAND(X1, X2)
  assign t1028 = ~(X1 & X2);

  // depth 1  NAND(X4, X5)
  assign t1029 = ~(X4 & X5);

  // depth 2  AND(t1028, t1029)
  assign t1030 = (t1028 & t1029);

  // depth 2  AND(X0, t999)
  assign t1031 = (X0 & t999);

  // depth 3  AND(t1030, t1031)
  assign t1032 = (t1030 & t1031);

  // depth 1  AND(X6, X7)
  assign t1033 = (X6 & X7);

  // depth 1  AND(X4, t908)
  assign t1034 = (X4 & t908);

  // depth 2  AND(t1033, t1034)
  assign t1035 = (t1033 & t1034);

  // depth 3  AND(t1035, t943)
  assign t1036 = (t1035 & t943);

  // depth 1  AND(X3, X4)
  assign t1037 = (X3 & X4);

  // depth 1  AND(X2, X5)
  assign t1038 = (X2 & X5);

  // depth 2  AND(t1037, t1038)
  assign t1039 = (t1037 & t1038);

  // depth 3  AND(t1039, t999)
  assign t1040 = (t1039 & t999);

  // depth 1  AND(X0, X1)
  assign t1041 = (X0 & X1);

  // depth 1  AND(X3, X5)
  assign t1042 = (X3 & X5);

  // depth 2  AND(X2, t1042)
  assign t1043 = (X2 & t1042);

  // depth 3  AND(t1041, t1043)
  assign t1044 = (t1041 & t1043);

  // depth 1  AND(X4, X6)
  assign t1045 = (X4 & X6);

  // depth 2  AND(X2, t1045)
  assign t1046 = (X2 & t1045);

  // depth 3  XOR2(t912, t916)
  assign t1047 = (t912 ^ t916);

  // depth 4  XOR2(t921, t925)
  assign t1048 = (t921 ^ t925);

  // depth 4  XOR2(t929, t934)
  assign t1049 = (t929 ^ t934);

  // depth 4  XOR2(t937, t942)
  assign t1050 = (t937 ^ t942);

  // depth 4  XOR2(t947, t951)
  assign t1051 = (t947 ^ t951);

  // depth 4  XOR2(t956, t960)
  assign t1052 = (t956 ^ t960);

  // depth 4  XOR2(t964, t968)
  assign t1053 = (t964 ^ t968);

  // depth 4  XOR2(t971, t975)
  assign t1054 = (t971 ^ t975);

  // depth 4  XOR2(t980, t985)
  assign t1055 = (t980 ^ t985);

  // depth 4  XOR2(t989, t993)
  assign t1056 = (t989 ^ t993);

  // depth 4  XOR2(t997, t1001)
  assign t1057 = (t997 ^ t1001);

  // depth 4  XOR2(t1005, t1010)
  assign t1058 = (t1005 ^ t1010);

  // depth 4  XOR2(t1017, t1022)
  assign t1059 = (t1017 ^ t1022);

  // depth 4  XOR2(t1027, t1032)
  assign t1060 = (t1027 ^ t1032);

  // depth 4  XOR2(t1036, t1040)
  assign t1061 = (t1036 ^ t1040);

  // depth 4  XOR2(t1044, t1046)
  assign t1062 = (t1044 ^ t1046);

  // depth 5  XOR2(t1047, t1048)
  assign t1063 = (t1047 ^ t1048);

  // depth 5  XOR2(t1049, t1050)
  assign t1064 = (t1049 ^ t1050);

  // depth 5  XOR2(t1051, t1052)
  assign t1065 = (t1051 ^ t1052);

  // depth 5  XOR2(t1053, t1054)
  assign t1066 = (t1053 ^ t1054);

  // depth 5  XOR2(t1055, t1056)
  assign t1067 = (t1055 ^ t1056);

  // depth 5  XOR2(t1057, t1058)
  assign t1068 = (t1057 ^ t1058);

  // depth 5  XOR2(t1059, t1060)
  assign t1069 = (t1059 ^ t1060);

  // depth 5  XOR2(t1061, t1062)
  assign t1070 = (t1061 ^ t1062);

  // depth 6  XOR2(t1063, t1064)
  assign t1071 = (t1063 ^ t1064);

  // depth 6  XOR2(t1065, t1066)
  assign t1072 = (t1065 ^ t1066);

  // depth 6  XOR2(t1067, t1068)
  assign t1073 = (t1067 ^ t1068);

  // depth 6  XOR2(t1069, t1070)
  assign t1074 = (t1069 ^ t1070);

  // depth 7  XOR2(t1071, t1072)
  assign t1075 = (t1071 ^ t1072);

  // depth 7  XOR2(t1073, t1074)
  assign t1076 = (t1073 ^ t1074);

  // depth 8  XOR2(t1075, t1076)
  assign t1077 = (t1075 ^ t1076);

  // depth 1  AND(X0, X1)
  assign t1078 = (X0 & X1);

  // depth 2  NAND(t1078, X5)
  assign t1079 = ~(t1078 & X5);

  // depth 0  NOT(X2)
  assign t1080 = ~X2;

  // depth 1  AND(X3, X4)
  assign t1081 = (X3 & X4);

  // depth 2  AND(t1081, t1080)
  assign t1082 = (t1081 & t1080);

  // depth 3  AND(t1079, t1082)
  assign t1083 = (t1079 & t1082);

  // depth 0  NOT(X1)
  assign t1084 = ~X1;

  // depth 0  NOT(X4)
  assign t1085 = ~X4;

  // depth 1  AND(X2, X6)
  assign t1086 = (X2 & X6);

  // depth 1  AND(X0, t1084)
  assign t1087 = (X0 & t1084);

  // depth 2  AND(t1086, t1087)
  assign t1088 = (t1086 & t1087);

  // depth 3  AND(t1088, t1085)
  assign t1089 = (t1088 & t1085);

  // depth 0  NOT(X3)
  assign t1090 = ~X3;

  // depth 0  NOT(X6)
  assign t1091 = ~X6;

  // depth 1  AND(X1, X5)
  assign t1092 = (X1 & X5);

  // depth 1  AND(X0, t1090)
  assign t1093 = (X0 & t1090);

  // depth 2  AND(t1092, t1093)
  assign t1094 = (t1092 & t1093);

  // depth 3  AND(t1094, t1091)
  assign t1095 = (t1094 & t1091);

  // depth 1  NAND(X1, X3)
  assign t1096 = ~(X1 & X3);

  // depth 1  AND(X2, X5)
  assign t1097 = (X2 & X5);

  // depth 2  AND(t1097, X6)
  assign t1098 = (t1097 & X6);

  // depth 3  AND(t1098, t1096)
  assign t1099 = (t1098 & t1096);

  // depth 0  NOT(X5)
  assign t1100 = ~X5;

  // depth 1  AND(X4, X6)
  assign t1101 = (X4 & X6);

  // depth 1  AND(X2, t1100)
  assign t1102 = (X2 & t1100);

  // depth 2  AND(t1101, t1102)
  assign t1103 = (t1101 & t1102);

  // depth 1  NAND(X0, X3)
  assign t1104 = ~(X0 & X3);

  // depth 1  NAND(X1, X6)
  assign t1105 = ~(X1 & X6);

  // depth 1  AND(X4, X5)
  assign t1106 = (X4 & X5);

  // depth 2  AND(t1106, t1104)
  assign t1107 = (t1106 & t1104);

  // depth 3  AND(t1107, t1105)
  assign t1108 = (t1107 & t1105);

  // depth 0  NOT(X0)
  assign t1109 = ~X0;

  // depth 1  NAND(X4, X5)
  assign t1110 = ~(X4 & X5);

  // depth 1  AND(X3, X6)
  assign t1111 = (X3 & X6);

  // depth 1  AND(X2, t1109)
  assign t1112 = (X2 & t1109);

  // depth 2  AND(t1111, t1112)
  assign t1113 = (t1111 & t1112);

  // depth 3  AND(t1113, t1110)
  assign t1114 = (t1113 & t1110);

  // depth 1  NAND(X0, X5)
  assign t1115 = ~(X0 & X5);

  // depth 1  AND(X3, X4)
  assign t1116 = (X3 & X4);

  // depth 1  AND(X2, X6)
  assign t1117 = (X2 & X6);

  // depth 2  AND(t1116, t1117)
  assign t1118 = (t1116 & t1117);

  // depth 2  AND(X1, t1115)
  assign t1119 = (X1 & t1115);

  // depth 3  AND(t1118, t1119)
  assign t1120 = (t1118 & t1119);

  // depth 0  NOT(X7)
  assign t1121 = ~X7;

  // depth 1  AND(X2, X4)
  assign t1122 = (X2 & X4);

  // depth 1  AND(X1, t1109)
  assign t1123 = (X1 & t1109);

  // depth 2  AND(t1122, t1123)
  assign t1124 = (t1122 & t1123);

  // depth 3  AND(t1124, t1121)
  assign t1125 = (t1124 & t1121);

  // depth 1  NAND(X1, X7)
  assign t1126 = ~(X1 & X7);

  // depth 1  AND(X3, X5)
  assign t1127 = (X3 & X5);

  // depth 1  AND(X2, t1109)
  assign t1128 = (X2 & t1109);

  // depth 2  AND(t1127, t1128)
  assign t1129 = (t1127 & t1128);

  // depth 3  AND(t1129, t1126)
  assign t1130 = (t1129 & t1126);

  // depth 1  NAND(X2, X7)
  assign t1131 = ~(X2 & X7);

  // depth 2  AND(X0, t1131)
  assign t1132 = (X0 & t1131);

  // depth 3  AND(t1132, t1110)
  assign t1133 = (t1132 & t1110);

  // depth 1  AND(X2, X5)
  assign t1134 = (X2 & X5);

  // depth 1  AND(X1, t1085)
  assign t1135 = (X1 & t1085);

  // depth 2  AND(t1134, t1135)
  assign t1136 = (t1134 & t1135);

  // depth 1  AND(X0, t1121)
  assign t1137 = (X0 & t1121);

  // depth 3  AND(t1136, t1137)
  assign t1138 = (t1136 & t1137);

  // depth 1  AND(t1109, t1090)
  assign t1139 = (t1109 & t1090);

  // depth 1  AND(X7, t1085)
  assign t1140 = (X7 & t1085);

  // depth 2  AND(t1139, t1140)
  assign t1141 = (t1139 & t1140);

  // depth 3  AND(t1141, t1100)
  assign t1142 = (t1141 & t1100);

  // depth 1  NAND(X2, X4)
  assign t1143 = ~(X2 & X4);

  // depth 1  AND(X3, X5)
  assign t1144 = (X3 & X5);

  // depth 1  AND(X1, X7)
  assign t1145 = (X1 & X7);

  // depth 2  AND(t1144, t1145)
  assign t1146 = (t1144 & t1145);

  // depth 3  AND(t1146, t1143)
  assign t1147 = (t1146 & t1143);

  // depth 1  AND(X0, X5)
  assign t1148 = (X0 & X5);

  // depth 2  NAND(t1148, X7)
  assign t1149 = ~(t1148 & X7);

  // depth 1  AND(X3, X4)
  assign t1150 = (X3 & X4);

  // depth 1  AND(X1, t1080)
  assign t1151 = (X1 & t1080);

  // depth 2  AND(t1150, t1151)
  assign t1152 = (t1150 & t1151);

  // depth 3  AND(t1149, t1152)
  assign t1153 = (t1149 & t1152);

  // depth 1  AND(X2, X3)
  assign t1154 = (X2 & X3);

  // depth 1  AND(X1, t1091)
  assign t1155 = (X1 & t1091);

  // depth 2  AND(t1154, t1155)
  assign t1156 = (t1154 & t1155);

  // depth 1  AND(X0, t1121)
  assign t1157 = (X0 & t1121);

  // depth 3  AND(t1156, t1157)
  assign t1158 = (t1156 & t1157);

  // depth 1  NAND(X4, X6)
  assign t1159 = ~(X4 & X6);

  // depth 1  AND(X0, X1)
  assign t1160 = (X0 & X1);

  // depth 2  AND(t1160, t1159)
  assign t1161 = (t1160 & t1159);

  // depth 3  AND(t1161, t1121)
  assign t1162 = (t1161 & t1121);

  // depth 1  AND(X0, X4)
  assign t1163 = (X0 & X4);

  // depth 2  NAND(t1163, X7)
  assign t1164 = ~(t1163 & X7);

  // depth 2  AND(X2, t1105)
  assign t1165 = (X2 & t1105);

  // depth 3  AND(t1164, t1165)
  assign t1166 = (t1164 & t1165);

  // depth 1  AND(X7, t1090)
  assign t1167 = (X7 & t1090);

  // depth 1  AND(X1, t1085)
  assign t1168 = (X1 & t1085);

  // depth 2  AND(t1167, t1168)
  assign t1169 = (t1167 & t1168);

  // depth 3  AND(t1169, t1091)
  assign t1170 = (t1169 & t1091);

  // depth 1  AND(X0, X1)
  assign t1171 = (X0 & X1);

  // depth 2  NAND(t1171, X4)
  assign t1172 = ~(t1171 & X4);

  // depth 1  AND(X3, X6)
  assign t1173 = (X3 & X6);

  // depth 2  AND(t1173, t1121)
  assign t1174 = (t1173 & t1121);

  // depth 3  AND(t1172, t1174)
  assign t1175 = (t1172 & t1174);

  // depth 1  AND(X2, X4)
  assign t1176 = (X2 & X4);

  // depth 2  NAND(t1176, X7)
  assign t1177 = ~(t1176 & X7);

  // depth 1  AND(X0, X3)
  assign t1178 = (X0 & X3);

  // depth 2  AND(t1178, t1091)
  assign t1179 = (t1178 & t1091);

  // depth 3  AND(t1177, t1179)
  assign t1180 = (t1177 & t1179);

  // depth 1  NAND(X6, X7)
  assign t1181 = ~(X6 & X7);

  // depth 1  AND(X1, X3)
  assign t1182 = (X1 & X3);

  // depth 2  AND(t1182, t1143)
  assign t1183 = (t1182 & t1143);

  // depth 2  AND(X0, t1181)
  assign t1184 = (X0 & t1181);

  // depth 3  AND(t1183, t1184)
  assign t1185 = (t1183 & t1184);

  // depth 1  AND(X6, t1080)
  assign t1186 = (X6 & t1080);

  // depth 1  AND(X5, t1090)
  assign t1187 = (X5 & t1090);

  // depth 2  AND(t1186, t1187)
  assign t1188 = (t1186 & t1187);

  // depth 1  AND(X0, t1121)
  assign t1189 = (X0 & t1121);

  // depth 3  AND(t1188, t1189)
  assign t1190 = (t1188 & t1189);

  // depth 1  NAND(X3, X5)
  assign t1191 = ~(X3 & X5);

  // depth 1  AND(X1, X2)
  assign t1192 = (X1 & X2);

  // depth 2  AND(t1192, t1191)
  assign t1193 = (t1192 & t1191);

  // depth 3  AND(t1193, t1181)
  assign t1194 = (t1193 & t1181);

  // depth 1  AND(X7, t1085)
  assign t1195 = (X7 & t1085);

  // depth 1  AND(X2, t1100)
  assign t1196 = (X2 & t1100);

  // depth 2  AND(t1195, t1196)
  assign t1197 = (t1195 & t1196);

  // depth 3  AND(t1197, t1091)
  assign t1198 = (t1197 & t1091);

  // depth 1  AND(X4, X5)
  assign t1199 = (X4 & X5);

  // depth 1  AND(X1, t1080)
  assign t1200 = (X1 & t1080);

  // depth 2  AND(t1199, t1200)
  assign t1201 = (t1199 & t1200);

  // depth 3  AND(t1201, t1181)
  assign t1202 = (t1201 & t1181);

  // depth 1  NAND(X1, X5)
  assign t1203 = ~(X1 & X5);

  // depth 1  AND(X4, X6)
  assign t1204 = (X4 & X6);

  // depth 2  AND(t1204, t1203)
  assign t1205 = (t1204 & t1203);

  // depth 2  AND(X0, t1131)
  assign t1206 = (X0 & t1131);

  // depth 3  AND(t1205, t1206)
  assign t1207 = (t1205 & t1206);

  // depth 1  AND(X6, X7)
  assign t1208 = (X6 & X7);

  // depth 2  AND(t1208, t1115)
  assign t1209 = (t1208 & t1115);

  // depth 1  AND(X4, t1090)
  assign t1210 = (X4 & t1090);

  // depth 3  AND(t1209, t1210)
  assign t1211 = (t1209 & t1210);

  // depth 1  AND(X4, X5)
  assign t1212 = (X4 & X5);

  // depth 1  AND(X3, t1084)
  assign t1213 = (X3 & t1084);

  // depth 2  AND(t1212, t1213)
  assign t1214 = (t1212 & t1213);

  // depth 3  AND(t1214, t1181)
  assign t1215 = (t1214 & t1181);

  // depth 1  NAND(X0, X7)
  assign t1216 = ~(X0 & X7);

  // depth 1  NAND(X3, X4)
  assign t1217 = ~(X3 & X4);

  // depth 2  AND(t1216, t1203)
  assign t1218 = (t1216 & t1203);

  // depth 2  AND(X6, t1217)
  assign t1219 = (X6 & t1217);

  // depth 3  AND(t1218, t1219)
  assign t1220 = (t1218 & t1219);

  // depth 1  AND(X5, X6)
  assign t1221 = (X5 & X6);

  // depth 2  AND(t1221, t1131)
  assign t1222 = (t1221 & t1131);

  // depth 1  AND(X3, t1085)
  assign t1223 = (X3 & t1085);

  // depth 3  AND(t1222, t1223)
  assign t1224 = (t1222 & t1223);

  // depth 1  AND(X3, X7)
  assign t1225 = (X3 & X7);

  // depth 2  AND(X2, t1225)
  assign t1226 = (X2 & t1225);

  // depth 4  XOR2(t1083, t1089)
  assign t1227 = (t1083 ^ t1089);

  // depth 4  XOR2(t1095, t1099)
  assign t1228 = (t1095 ^ t1099);

  // depth 4  XOR2(t1103, t1108)
  assign t1229 = (t1103 ^ t1108);

  // depth 4  XOR2(t1114, t1120)
  assign t1230 = (t1114 ^ t1120);

  // depth 4  XOR2(t1125, t1130)
  assign t1231 = (t1125 ^ t1130);

  // depth 4  XOR2(t1133, t1138)
  assign t1232 = (t1133 ^ t1138);

  // depth 4  XOR2(t1142, t1147)
  assign t1233 = (t1142 ^ t1147);

  // depth 4  XOR2(t1153, t1158)
  assign t1234 = (t1153 ^ t1158);

  // depth 4  XOR2(t1162, t1166)
  assign t1235 = (t1162 ^ t1166);

  // depth 4  XOR2(t1170, t1175)
  assign t1236 = (t1170 ^ t1175);

  // depth 4  XOR2(t1180, t1185)
  assign t1237 = (t1180 ^ t1185);

  // depth 4  XOR2(t1190, t1194)
  assign t1238 = (t1190 ^ t1194);

  // depth 4  XOR2(t1198, t1202)
  assign t1239 = (t1198 ^ t1202);

  // depth 4  XOR2(t1207, t1211)
  assign t1240 = (t1207 ^ t1211);

  // depth 4  XOR2(t1215, t1220)
  assign t1241 = (t1215 ^ t1220);

  // depth 4  XOR2(t1224, t1226)
  assign t1242 = (t1224 ^ t1226);

  // depth 5  XOR2(t1227, t1228)
  assign t1243 = (t1227 ^ t1228);

  // depth 5  XOR2(t1229, t1230)
  assign t1244 = (t1229 ^ t1230);

  // depth 5  XOR2(t1231, t1232)
  assign t1245 = (t1231 ^ t1232);

  // depth 5  XOR2(t1233, t1234)
  assign t1246 = (t1233 ^ t1234);

  // depth 5  XOR2(t1235, t1236)
  assign t1247 = (t1235 ^ t1236);

  // depth 5  XOR2(t1237, t1238)
  assign t1248 = (t1237 ^ t1238);

  // depth 5  XOR2(t1239, t1240)
  assign t1249 = (t1239 ^ t1240);

  // depth 5  XOR2(t1241, t1242)
  assign t1250 = (t1241 ^ t1242);

  // depth 6  XOR2(t1243, t1244)
  assign t1251 = (t1243 ^ t1244);

  // depth 6  XOR2(t1245, t1246)
  assign t1252 = (t1245 ^ t1246);

  // depth 6  XOR2(t1247, t1248)
  assign t1253 = (t1247 ^ t1248);

  // depth 6  XOR2(t1249, t1250)
  assign t1254 = (t1249 ^ t1250);

  // depth 7  XOR2(t1251, t1252)
  assign t1255 = (t1251 ^ t1252);

  // depth 7  XOR2(t1253, t1254)
  assign t1256 = (t1253 ^ t1254);

  // depth 8  XOR2(t1255, t1256)
  assign t1257 = (t1255 ^ t1256);

  // depth 0  NOT(X1)
  assign t1258 = ~X1;

  // depth 1  NAND(X3, X5)
  assign t1259 = ~(X3 & X5);

  // depth 1  AND(X2, X4)
  assign t1260 = (X2 & X4);

  // depth 1  AND(X0, t1258)
  assign t1261 = (X0 & t1258);

  // depth 2  AND(t1260, t1261)
  assign t1262 = (t1260 & t1261);

  // depth 3  AND(t1262, t1259)
  assign t1263 = (t1262 & t1259);

  // depth 0  NOT(X4)
  assign t1264 = ~X4;

  // depth 1  AND(X2, X3)
  assign t1265 = (X2 & X3);

  // depth 1  AND(X0, X6)
  assign t1266 = (X0 & X6);

  // depth 2  AND(t1265, t1266)
  assign t1267 = (t1265 & t1266);

  // depth 3  AND(t1267, t1264)
  assign t1268 = (t1267 & t1264);

  // depth 1  NAND(X3, X4)
  assign t1269 = ~(X3 & X4);

  // depth 1  AND(X1, X2)
  assign t1270 = (X1 & X2);

  // depth 1  AND(X0, X6)
  assign t1271 = (X0 & X6);

  // depth 2  AND(t1270, t1271)
  assign t1272 = (t1270 & t1271);

  // depth 3  AND(t1272, t1269)
  assign t1273 = (t1272 & t1269);

  // depth 1  NAND(X0, X6)
  assign t1274 = ~(X0 & X6);

  // depth 1  AND(X2, X3)
  assign t1275 = (X2 & X3);

  // depth 1  AND(X1, X5)
  assign t1276 = (X1 & X5);

  // depth 2  AND(t1275, t1276)
  assign t1277 = (t1275 & t1276);

  // depth 3  AND(t1277, t1274)
  assign t1278 = (t1277 & t1274);

  // depth 1  NAND(X0, X2)
  assign t1279 = ~(X0 & X2);

  // depth 1  AND(X4, X5)
  assign t1280 = (X4 & X5);

  // depth 2  AND(t1280, X6)
  assign t1281 = (t1280 & X6);

  // depth 3  AND(t1281, t1279)
  assign t1282 = (t1281 & t1279);

  // depth 0  NOT(X2)
  assign t1283 = ~X2;

  // depth 1  AND(X4, X5)
  assign t1284 = (X4 & X5);

  // depth 1  AND(X1, X6)
  assign t1285 = (X1 & X6);

  // depth 2  AND(t1284, t1285)
  assign t1286 = (t1284 & t1285);

  // depth 3  AND(t1286, t1283)
  assign t1287 = (t1286 & t1283);

  // depth 1  NAND(X0, X5)
  assign t1288 = ~(X0 & X5);

  // depth 1  AND(X3, X6)
  assign t1289 = (X3 & X6);

  // depth 2  AND(t1289, t1288)
  assign t1290 = (t1289 & t1288);

  // depth 1  AND(X1, t1264)
  assign t1291 = (X1 & t1264);

  // depth 3  AND(t1290, t1291)
  assign t1292 = (t1290 & t1291);

  // depth 1  NAND(X2, X7)
  assign t1293 = ~(X2 & X7);

  // depth 1  AND(X0, X3)
  assign t1294 = (X0 & X3);

  // depth 2  AND(t1294, X4)
  assign t1295 = (t1294 & X4);

  // depth 3  AND(t1295, t1293)
  assign t1296 = (t1295 & t1293);

  // depth 1  NAND(X3, X7)
  assign t1297 = ~(X3 & X7);

  // depth 0  NOT(X5)
  assign t1298 = ~X5;

  // depth 1  AND(X0, X1)
  assign t1299 = (X0 & X1);

  // depth 2  AND(t1299, t1297)
  assign t1300 = (t1299 & t1297);

  // depth 3  AND(t1300, t1298)
  assign t1301 = (t1300 & t1298);

  // depth 0  NOT(X3)
  assign t1302 = ~X3;

  // depth 1  AND(X5, t1302)
  assign t1303 = (X5 & t1302);

  // depth 2  AND(t1293, t1303)
  assign t1304 = (t1293 & t1303);

  // depth 0  NOT(X7)
  assign t1305 = ~X7;

  // depth 1  AND(X5, t1283)
  assign t1306 = (X5 & t1283);

  // depth 1  AND(X0, t1302)
  assign t1307 = (X0 & t1302);

  // depth 2  AND(t1306, t1307)
  assign t1308 = (t1306 & t1307);

  // depth 3  AND(t1308, t1305)
  assign t1309 = (t1308 & t1305);

  // depth 1  NAND(X0, X1)
  assign t1310 = ~(X0 & X1);

  // depth 2  AND(t1310, t1293)
  assign t1311 = (t1310 & t1293);

  // depth 1  AND(X4, t1298)
  assign t1312 = (X4 & t1298);

  // depth 3  AND(t1311, t1312)
  assign t1313 = (t1311 & t1312);

  // depth 0  NOT(X0)
  assign t1314 = ~X0;

  // depth 1  AND(X4, X5)
  assign t1315 = (X4 & X5);

  // depth 1  AND(X2, t1314)
  assign t1316 = (X2 & t1314);

  // depth 2  AND(t1315, t1316)
  assign t1317 = (t1315 & t1316);

  // depth 3  AND(t1317, t1297)
  assign t1318 = (t1317 & t1297);

  // depth 1  AND(X2, X4)
  assign t1319 = (X2 & X4);

  // depth 2  NAND(t1319, X5)
  assign t1320 = ~(t1319 & X5);

  // depth 1  AND(X1, X3)
  assign t1321 = (X1 & X3);

  // depth 2  AND(t1321, t1305)
  assign t1322 = (t1321 & t1305);

  // depth 3  AND(t1320, t1322)
  assign t1323 = (t1320 & t1322);

  // depth 1  NAND(X4, X7)
  assign t1324 = ~(X4 & X7);

  // depth 1  AND(X2, X3)
  assign t1325 = (X2 & X3);

  // depth 2  AND(t1325, t1288)
  assign t1326 = (t1325 & t1288);

  // depth 2  AND(X1, t1324)
  assign t1327 = (X1 & t1324);

  // depth 3  AND(t1326, t1327)
  assign t1328 = (t1326 & t1327);

  // depth 1  AND(X0, X1)
  assign t1329 = (X0 & X1);

  // depth 2  NAND(t1329, X2)
  assign t1330 = ~(t1329 & X2);

  // depth 1  NAND(X6, X7)
  assign t1331 = ~(X6 & X7);

  // depth 3  AND(t1330, t1331)
  assign t1332 = (t1330 & t1331);

  // depth 1  NAND(X2, X3)
  assign t1333 = ~(X2 & X3);

  // depth 0  NOT(X6)
  assign t1334 = ~X6;

  // depth 1  AND(X1, X7)
  assign t1335 = (X1 & X7);

  // depth 2  AND(t1335, t1333)
  assign t1336 = (t1335 & t1333);

  // depth 3  AND(t1336, t1334)
  assign t1337 = (t1336 & t1334);

  // depth 1  AND(X0, X4)
  assign t1338 = (X0 & X4);

  // depth 2  AND(t1338, X6)
  assign t1339 = (t1338 & X6);

  // depth 3  AND(t1339, t1293)
  assign t1340 = (t1339 & t1293);

  // depth 1  AND(X1, X2)
  assign t1341 = (X1 & X2);

  // depth 2  AND(t1341, t1324)
  assign t1342 = (t1341 & t1324);

  // depth 3  AND(t1342, t1334)
  assign t1343 = (t1342 & t1334);

  // depth 1  NAND(X1, X4)
  assign t1344 = ~(X1 & X4);

  // depth 1  AND(X7, t1314)
  assign t1345 = (X7 & t1314);

  // depth 2  AND(t1345, t1344)
  assign t1346 = (t1345 & t1344);

  // depth 1  AND(X3, t1334)
  assign t1347 = (X3 & t1334);

  // depth 3  AND(t1346, t1347)
  assign t1348 = (t1346 & t1347);

  // depth 1  AND(X5, X6)
  assign t1349 = (X5 & X6);

  // depth 2  NAND(t1349, X7)
  assign t1350 = ~(t1349 & X7);

  // depth 1  AND(X0, X2)
  assign t1351 = (X0 & X2);

  // depth 3  AND(t1351, t1350)
  assign t1352 = (t1351 & t1350);

  // depth 1  AND(X5, t1283)
  assign t1353 = (X5 & t1283);

  // depth 2  AND(t1274, t1353)
  assign t1354 = (t1274 & t1353);

  // depth 1  AND(X1, t1305)
  assign t1355 = (X1 & t1305);

  // depth 3  AND(t1354, t1355)
  assign t1356 = (t1354 & t1355);

  // depth 1  AND(X3, X6)
  assign t1357 = (X3 & X6);

  // depth 2  AND(t1357, t1293)
  assign t1358 = (t1357 & t1293);

  // depth 3  AND(t1358, t1298)
  assign t1359 = (t1358 & t1298);

  // depth 1  NAND(X0, X3)
  assign t1360 = ~(X0 & X3);

  // depth 1  NAND(X5, X6)
  assign t1361 = ~(X5 & X6);

  // depth 1  AND(X2, X7)
  assign t1362 = (X2 & X7);

  // depth 2  AND(t1362, t1360)
  assign t1363 = (t1362 & t1360);

  // depth 3  AND(t1363, t1361)
  assign t1364 = (t1363 & t1361);

  // depth 1  NAND(X0, X7)
  assign t1365 = ~(X0 & X7);

  // depth 1  AND(X3, X6)
  assign t1366 = (X3 & X6);

  // depth 2  AND(t1366, t1365)
  assign t1367 = (t1366 & t1365);

  // depth 1  AND(X1, X2)
  assign t1368 = (X1 & X2);

  // depth 2  AND(t1368, t1298)
  assign t1369 = (t1368 & t1298);

  // depth 3  AND(t1367, t1369)
  assign t1370 = (t1367 & t1369);

  // depth 1  AND(X4, X5)
  assign t1371 = (X4 & X5);

  // depth 1  AND(X0, t1334)
  assign t1372 = (X0 & t1334);

  // depth 2  AND(t1371, t1372)
  assign t1373 = (t1371 & t1372);

  // depth 3  AND(t1373, t1305)
  assign t1374 = (t1373 & t1305);

  // depth 1  NAND(X0, X4)
  assign t1375 = ~(X0 & X4);

  // depth 1  NAND(X5, X7)
  assign t1376 = ~(X5 & X7);

  // depth 1  AND(X1, X6)
  assign t1377 = (X1 & X6);

  // depth 2  AND(t1377, t1375)
  assign t1378 = (t1377 & t1375);

  // depth 3  AND(t1378, t1376)
  assign t1379 = (t1378 & t1376);

  // depth 1  NAND(X2, X4)
  assign t1380 = ~(X2 & X4);

  // depth 1  AND(X5, X6)
  assign t1381 = (X5 & X6);

  // depth 2  AND(t1381, t1365)
  assign t1382 = (t1381 & t1365);

  // depth 3  AND(t1382, t1380)
  assign t1383 = (t1382 & t1380);

  // depth 1  AND(X4, X6)
  assign t1384 = (X4 & X6);

  // depth 2  AND(t1384, t1302)
  assign t1385 = (t1384 & t1302);

  // depth 3  AND(t1385, t1376)
  assign t1386 = (t1385 & t1376);

  // depth 1  AND(X0, X5)
  assign t1387 = (X0 & X5);

  // depth 2  NAND(t1387, X6)
  assign t1388 = ~(t1387 & X6);

  // depth 2  AND(X3, t1324)
  assign t1389 = (X3 & t1324);

  // depth 3  AND(t1388, t1389)
  assign t1390 = (t1388 & t1389);

  // depth 1  AND(X5, X6)
  assign t1391 = (X5 & X6);

  // depth 1  AND(X1, t1302)
  assign t1392 = (X1 & t1302);

  // depth 2  AND(t1391, t1392)
  assign t1393 = (t1391 & t1392);

  // depth 3  AND(t1393, t1324)
  assign t1394 = (t1393 & t1324);

  // depth 2  AND(t1274, t1344)
  assign t1395 = (t1274 & t1344);

  // depth 2  AND(X7, t1259)
  assign t1396 = (X7 & t1259);

  // depth 3  AND(t1395, t1396)
  assign t1397 = (t1395 & t1396);

  // depth 4  XOR2(t1263, t1268)
  assign t1398 = (t1263 ^ t1268);

  // depth 4  XOR2(t1273, t1278)
  assign t1399 = (t1273 ^ t1278);

  // depth 4  XOR2(t1282, t1287)
  assign t1400 = (t1282 ^ t1287);

  // depth 4  XOR2(t1292, t1296)
  assign t1401 = (t1292 ^ t1296);

  // depth 4  XOR2(t1301, t1304)
  assign t1402 = (t1301 ^ t1304);

  // depth 4  XOR2(t1309, t1313)
  assign t1403 = (t1309 ^ t1313);

  // depth 4  XOR2(t1318, t1323)
  assign t1404 = (t1318 ^ t1323);

  // depth 4  XOR2(t1328, t1332)
  assign t1405 = (t1328 ^ t1332);

  // depth 4  XOR2(t1337, t1340)
  assign t1406 = (t1337 ^ t1340);

  // depth 4  XOR2(t1343, t1348)
  assign t1407 = (t1343 ^ t1348);

  // depth 4  XOR2(t1352, t1356)
  assign t1408 = (t1352 ^ t1356);

  // depth 4  XOR2(t1359, t1364)
  assign t1409 = (t1359 ^ t1364);

  // depth 4  XOR2(t1370, t1374)
  assign t1410 = (t1370 ^ t1374);

  // depth 4  XOR2(t1379, t1383)
  assign t1411 = (t1379 ^ t1383);

  // depth 4  XOR2(t1386, t1390)
  assign t1412 = (t1386 ^ t1390);

  // depth 4  XOR2(t1394, t1397)
  assign t1413 = (t1394 ^ t1397);

  // depth 5  XOR2(t1398, t1399)
  assign t1414 = (t1398 ^ t1399);

  // depth 5  XOR2(t1400, t1401)
  assign t1415 = (t1400 ^ t1401);

  // depth 5  XOR2(t1402, t1403)
  assign t1416 = (t1402 ^ t1403);

  // depth 5  XOR2(t1404, t1405)
  assign t1417 = (t1404 ^ t1405);

  // depth 5  XOR2(t1406, t1407)
  assign t1418 = (t1406 ^ t1407);

  // depth 5  XOR2(t1408, t1409)
  assign t1419 = (t1408 ^ t1409);

  // depth 5  XOR2(t1410, t1411)
  assign t1420 = (t1410 ^ t1411);

  // depth 5  XOR2(t1412, t1413)
  assign t1421 = (t1412 ^ t1413);

  // depth 6  XOR2(t1414, t1415)
  assign t1422 = (t1414 ^ t1415);

  // depth 6  XOR2(t1416, t1417)
  assign t1423 = (t1416 ^ t1417);

  // depth 6  XOR2(t1418, t1419)
  assign t1424 = (t1418 ^ t1419);

  // depth 6  XOR2(t1420, t1421)
  assign t1425 = (t1420 ^ t1421);

  // depth 7  XOR2(t1422, t1423)
  assign t1426 = (t1422 ^ t1423);

  // depth 7  XOR2(t1424, t1425)
  assign t1427 = (t1424 ^ t1425);

  // depth 8  XOR2(t1426, t1427)
  assign t1428 = (t1426 ^ t1427);

  assign Y0 = t734;
  assign Y1 = t907;
  assign Y2 = t1077;
  assign Y3 = t185;
  assign Y4 = t377;
  assign Y5 = t1257;
  assign Y6 = t566;
  assign Y7 = t1428;

endmodule


// 8-bit bus wrapper used by the waveform truth-table testbench.
module AES(
    input  wire [7:0] X,
    output wire [7:0] F
);

  AES_SBOX_TGATES u_aes_sbox_tgates (
      .X0(X[7]),
      .X1(X[6]),
      .X2(X[5]),
      .X3(X[4]),
      .X4(X[3]),
      .X5(X[2]),
      .X6(X[1]),
      .X7(X[0]),
      .Y0(F[7]),
      .Y1(F[6]),
      .Y2(F[5]),
      .Y3(F[4]),
      .Y4(F[3]),
      .Y5(F[2]),
      .Y6(F[1]),
      .Y7(F[0])
  );

endmodule
