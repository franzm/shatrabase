/***************************************************************************
 *                   Shatra pieces for Povray                              *
 *   (C) 2014 stefan.berke @ modular-audio-graphics.com                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

//+W6000 +H1000 +D +P +A0.25 +AM2 +R2 

#version 3.7;

camera 
{
	orthographic
	
	location  z * -1
	look_at   0
	right     2.8
	angle     136.5
	translate x * 2
}

default
{
	finish { diffuse 0 ambient 1 }
}

#declare pieceRadius = 0.3;
#declare ringRadius = pieceRadius / 14;

#declare High = pigment { color rgb 1 };
#declare Mid = pigment { color rgb 0.9 };
#declare Low = pigment { color rgb 0.7 };


#declare Piece = union 
{
	// piece itself
	cylinder { z*0.1, z, pieceRadius pigment { Mid } }
	// outer ring
	torus 
	{ 
		pieceRadius, ringRadius
		rotate x * 90
		pigment { High } 
	} 
};

#declare Tura = union
{
	object { Piece }
	box { -1,1 scale pieceRadius / 2 pigment { Low } }
}

#declare Yalkyn = union
{
	object { Piece }
	cylinder { x*-1, x*1, 0.3 scale pieceRadius-ringRadius scale <1,1,0.01> pigment { Low } }
}

#macro trif(ang) <sin(ang*2*pi),cos(ang*2*pi),0> #end

#declare Batyr = union
{
	object { Piece }
	triangle { trif(0), trif(0.333), trif(0.666) scale pieceRadius*0.7 pigment { Low } }
}

#declare Biy = union
{
	object { Piece }
	cylinder { 0,z, pieceRadius * 0.6 pigment { Low } }
}

#declare PieceRow = union
{
	object { Tura }
	object { Yalkyn translate x * 1 }
	object { Batyr translate x * 2 }
	object { Biy translate x * 3 }
	object { Piece translate x * 4 }
}

object { PieceRow pigment { rgb 1 } }
	
	

