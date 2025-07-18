xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 16;
 37.58924;39.94245;-54.12104;,
 110.47168;22.80311;-85.19911;,
 8.51796;5.15634;-21.56561;,
 88.29551;-55.81519;-39.89213;,
 33.53829;-32.60845;-10.07236;,
 97.76292;65.62326;-22.09111;,
 160.11237;22.00132;-67.76491;,
 126.16365;70.59097;-3.79764;,
 189.69484;-14.17766;-33.70061;,
 33.53829;-32.60845;-10.07236;,
 88.29551;-55.81519;-39.89213;,
 130.12543;-30.36857;52.14981;,
 168.54352;-54.37692;-32.80280;,
 -2.18907;0.73889;-0.55024;,
 45.30463;30.16873;8.69126;,
 108.68380;54.03934;47.11616;;
 
 22;
 3;0,1,2;,
 3;1,3,2;,
 3;2,3,4;,
 4;1,0,5,6;,
 4;6,5,7,8;,
 3;9,10,11;,
 3;10,12,11;,
 3;11,12,7;,
 3;7,12,8;,
 3;4,13,2;,
 3;2,13,0;,
 3;13,14,0;,
 3;0,14,5;,
 3;14,15,5;,
 3;5,15,7;,
 3;10,1,12;,
 3;12,1,8;,
 3;8,1,6;,
 3;13,9,14;,
 3;9,11,14;,
 3;14,11,15;,
 3;15,11,7;;
 
 MeshMaterialList {
  1;
  22;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  31;
  -0.328766;-0.499496;-0.801509;,
  -0.019628;-0.440764;-0.897409;,
  0.317562;0.837218;-0.445219;,
  0.538171;0.783839;-0.309787;,
  -0.006523;-0.963057;0.269219;,
  -0.619519;-0.573556;-0.535937;,
  -0.658724;-0.331558;-0.675391;,
  -0.742632;-0.006100;-0.669672;,
  0.120256;0.961384;-0.247546;,
  0.869112;0.468910;0.157375;,
  -0.104282;-0.945404;0.308766;,
  -0.874922;-0.116045;-0.470153;,
  -0.490458;0.847990;0.200909;,
  -0.376626;0.902148;0.210433;,
  0.074768;0.832110;-0.549548;,
  -0.349618;0.934902;-0.061044;,
  -0.282427;-0.512628;0.810831;,
  0.899344;0.238406;0.366529;,
  0.674318;0.057812;0.736175;,
  0.279856;-0.125993;0.951739;,
  -0.543717;0.816575;0.193847;,
  -0.463114;0.862467;0.204149;,
  -0.293966;0.934068;0.202732;,
  0.270069;-0.393402;-0.878805;,
  0.362571;-0.397269;-0.843042;,
  0.272994;-0.531668;-0.801750;,
  -0.010165;-0.284610;0.958590;,
  -0.358785;-0.175371;0.916798;,
  -0.515209;-0.111869;0.849733;,
  -0.496325;-0.074498;0.864934;,
  0.888408;0.248687;0.385857;;
  22;
  3;7,1,6;,
  3;1,0,6;,
  3;6,0,5;,
  4;14,15,8,2;,
  4;2,8,9,3;,
  3;16,10,10;,
  3;10,4,10;,
  3;17,18,9;,
  3;19,18,19;,
  3;5,11,6;,
  3;6,11,7;,
  3;20,12,15;,
  3;15,12,21;,
  3;12,13,21;,
  3;8,13,22;,
  3;0,1,23;,
  3;23,1,24;,
  3;24,1,25;,
  3;26,16,27;,
  3;16,28,27;,
  3;27,28,29;,
  3;30,17,9;;
 }
 MeshTextureCoords {
  16;
  0.333330;0.500000;,
  0.333330;0.250000;,
  0.111110;0.500000;,
  0.000000;0.250000;,
  0.000000;0.500000;,
  0.444440;0.500000;,
  0.444440;0.250000;,
  0.555560;0.500000;,
  0.555560;0.250000;,
  1.000000;0.500000;,
  1.000000;0.250000;,
  0.777780;0.500000;,
  0.777780;0.250000;,
  0.111110;0.750000;,
  0.222220;0.750000;,
  0.444440;0.750000;;
 }
}
