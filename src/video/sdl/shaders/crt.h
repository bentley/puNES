/*
 *  Copyright (C) 2010-2016 Fabio Cavallo (aka FHorse)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*****************************************************************************************/
/* CRT                                                                                   */
/*****************************************************************************************/
{
	// vertex shader
	"varying float CRTgamma;\n"
	"varying float monitorgamma;\n"
	"varying vec2 overscan;\n"
	"varying vec2 aspect;\n"
	"varying float d;\n"
	"varying float R;\n"
	"varying float cornersize;\n"
	"varying float cornersmooth;\n"

	"varying vec3 stretch;\n"
	"varying vec2 sinangle;\n"
	"varying vec2 cosangle;\n"

	"uniform vec2 size_screen_emu;\n"
	"uniform vec2 size_texture;\n"
	"uniform float pixel_aspect_ratio;\n"

	"varying vec2 texCoord;\n"
	"varying vec2 one;\n"
	"varying float mod_factor;\n"

	"#define FIX(c) max(abs(c), 1e-5);\n"

	"float intersect(vec2 xy) {\n"
	"	float A = dot(xy,xy)+d*d;\n"
	"	float B = 2.0*(R*(dot(xy,sinangle)-d*cosangle.x*cosangle.y)-d*d);\n"
	"	float C = d*d + 2.0*R*d*cosangle.x*cosangle.y;\n"
	"	return (-B-sqrt(B*B-4.0*A*C))/(2.0*A);\n"
	"}\n"

	"vec2 bkwtrans(vec2 xy) {\n"
	"	float c = intersect(xy);\n"
	"	vec2 point = c*xy;\n"
	"	point -= -R*sinangle;\n"
	"	point /= vec2(R);\n"
	"	vec2 tang = sinangle/cosangle;\n"
	"	vec2 poc = point/cosangle;\n"
	"	float A = dot(tang,tang)+1.0;\n"
	"	float B = -2.0*dot(poc,tang);\n"
	"	float C = dot(poc,poc)-1.0;\n"
	"	float a = (-B+sqrt(B*B-4.0*A*C))/(2.0*A);\n"
	"	vec2 uv = (point-a*sinangle)/cosangle;\n"
	"	float r = R*acos(a);\n"
	"	return uv*r/sin(r/R);\n"
	"}\n"

	"vec2 fwtrans(vec2 uv) {\n"
	"	float r = FIX(sqrt(dot(uv,uv)));\n"
	"	uv *= sin(r/R)/r;\n"
	"	float x = 1.0-cos(r/R);\n"
	"	float D = d/R + x*cosangle.x*cosangle.y+dot(uv,sinangle);\n"
	"	return d*(uv*cosangle-x*sinangle)/D;\n"
	"}\n"

	"vec3 maxscale() {\n"
	"	vec2 c = bkwtrans(-R * sinangle / (1.0 + R/d*cosangle.x*cosangle.y));\n"
	"	vec2 a = 0.5*aspect;\n"
	"	vec2 lo = vec2(fwtrans(vec2(-a.x,c.y)).x,\n"
	"			fwtrans(vec2(c.x,-a.y)).y)/aspect;\n"
	"	vec2 hi = vec2(fwtrans(vec2(+a.x,c.y)).x,\n"
	"			fwtrans(vec2(c.x,+a.y)).y)/aspect;\n"
	"	return vec3((hi+lo)*aspect*0.5,max(hi.x-lo.x,hi.y-lo.y));\n"
	"}\n"

	"void main() {\n"
	"	gl_FrontColor = gl_Color;\n"
	"	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
	"	texCoord = gl_MultiTexCoord0.xy;\n"

	"	// START of parameters\n"

	"	// gamma of simulated CRT\n"
	"	CRTgamma = 2.4;\n"
	"	// gamma of display monitor (typically 2.2 is correct)\n"
	"	monitorgamma = 2.2;\n"
	"	// overscan (e.g. 1.02 for 2% overscan)\n"
	"	overscan = vec2(1.01,1.01);\n"
	"	// aspect ratio\n"
	"	aspect = vec2(1.0, 0.75);\n"
	"	// lengths are measured in units of (approximately) the width\n"
	"	// of the monitor simulated distance from viewer to monitor\n"
	"	d = 2.0;\n"
	"	// radius of curvature\n"
	"	R = 1.5;\n"
	"	// tilt angle in radians\n"
	"	// (behavior might be a bit wrong if both components are\n"
	"	// nonzero)\n"
	"	const vec2 angle = vec2(0.0,-0.15);\n"
	"	// size of curved corners\n"
	"	cornersize = 0.03;\n"
	"	// border smoothness parameter\n"
	"	// decrease if borders are too aliased\n"
	"	cornersmooth = 1000.0;\n"

	"	// END of parameters\n"

	"	// Precalculate a bunch of useful values we'll need in the fragment shader.\n"
	"	sinangle = sin(angle);\n"
	"	cosangle = cos(angle);\n"
	"	stretch = maxscale();\n"

	"	// The size of one texel, in texture-coordinates.\n"
	"	one = 1.0 / (vec2(256.0) * (size_screen_emu.x / size_screen_emu.y));\n"

	"	// Resulting X pixel-coordinate of the pixel we're drawing.\n"
	"	mod_factor = texCoord.x * (256.0 * pixel_aspect_ratio);\n"
	"}",
	// fragment shader
	"// Macros.\n"
	"#define FIX(c) max(abs(c), 1e-5);\n"
	"#define PI 3.141592653589\n"

	"uniform vec2 size_screen_emu;\n"
	"uniform vec2 size_texture;\n"
	"uniform float param;\n"
	"uniform float full_interpolation;\n"

	"uniform sampler2D texture_scr;\n"

	"varying vec2 texCoord;\n"
	"varying vec2 one;\n"
	"varying float mod_factor;\n"

	"varying float CRTgamma;\n"
	"varying float monitorgamma;\n"

	"varying vec2 overscan;\n"
	"varying vec2 aspect;\n"

	"varying float d;\n"
	"varying float R;\n"

	"varying float cornersize;\n"
	"varying float cornersmooth;\n"

	"varying vec3 stretch;\n"
	"varying vec2 sinangle;\n"
	"varying vec2 cosangle;\n"

	"float intersect(vec2 xy) {\n"
	"	float A = dot(xy,xy)+d*d;\n"
	"	float B = 2.0*(R*(dot(xy,sinangle)-d*cosangle.x*cosangle.y)-d*d);\n"
	"	float C = d*d + 2.0*R*d*cosangle.x*cosangle.y;\n"
	"	return (-B-sqrt(B*B-4.0*A*C))/(2.0*A);\n"
	"}\n"

	"vec2 bkwtrans(vec2 xy) {\n"
	"	float c = intersect(xy);\n"
	"	vec2 point = c*xy;\n"
	"	point -= -R*sinangle;\n"
	"	point /= vec2(R);\n"
	"	vec2 tang = sinangle/cosangle;\n"
	"	vec2 poc = point/cosangle;\n"
	"	float A = dot(tang,tang)+1.0;\n"
	"	float B = -2.0*dot(poc,tang);\n"
	"	float C = dot(poc,poc)-1.0;\n"
	"	float a = (-B+sqrt(B*B-4.0*A*C))/(2.0*A);\n"
	"	vec2 uv = (point-a*sinangle)/cosangle;\n"
	"	float r = FIX(R*acos(a));\n"
	"	return uv*r/sin(r/R);\n"
	"}\n"

	"vec2 transform(vec2 coord) {\n"
	"	coord *= (256.0 / size_screen_emu);\n"
	"	coord = (coord-0.5)*aspect*stretch.z+stretch.xy;\n"
	"	return (bkwtrans(coord)/overscan/aspect+0.5) * (size_screen_emu / 256.0);\n"
	"}\n"

	"float corner(vec2 coord) {\n"
	"	coord *= (256.0 / size_screen_emu);\n"
	"	coord = (coord - 0.5) * overscan + 0.5;\n"
	"	coord = min(coord, 1.0-coord) * aspect;\n"
	"	vec2 cdist = vec2(cornersize);\n"
	"	coord = (cdist - min(coord,cdist));\n"
	"	float dist = sqrt(dot(coord,coord));\n"
	"	return clamp((cdist.x-dist)*cornersmooth,0.0, 1.0);\n"
	"}\n"

	"// Calculate the influence of a scanline on the current pixel.\n"
	"// 'distance' is the distance in texture coordinates from the current\n"
	"// pixel to the scanline in question.\n"
	"// 'color' is the colour of the scanline at the horizontal location of\n"
	"// the current pixel.\n"
	"vec4 scanlineWeights(float distance, vec4 color) {\n"
	"	// 'wid' controls the width of the scanline beam, for each RGB\n"
	"	// channel The 'weights' lines basically specify the formula\n"
	"	// that gives you the profile of the beam, i.e. the intensity as\n"
	"	// a function of distance from the vertical center of the\n"
	"	// scanline. In this case, it is gaussian if width=2, and\n"
	"	// becomes nongaussian for larger widths. Ideally this should\n"
	"	// be normalized so that the integral across the beam is\n"
	"	// independent of its width. That is, for a narrower beam\n"
	"	// 'weights' should have a higher peak at the center of the\n"
	"	// scanline than for a wider beam.\n"
	"	if (param == 1.0) {\n"
	"		// Use the older, purely gaussian beam profile\n"
	"		vec4 wid = 0.3 + 0.1 * pow(color, vec4(3.0));\n"
	"		vec4 weights = vec4(distance / wid);\n"
	"		return 0.4 * exp(-weights * weights) / wid;\n"
	"	} else {\n"
	"		vec4 wid = 2.0 + 2.0 * pow(color, vec4(4.0));\n"
	"		vec4 weights = vec4(distance / 0.3);\n"
	"		return 1.4 * exp(-pow(weights * inversesqrt(0.5 * wid), wid)) / (0.6 + 0.2 * wid);\n"
	"	}\n"
	"}\n"

	"void main() {\n"
	"	vec2 xy;\n"
	"	vec4 col;\n"
	"	vec4 col2;\n"

	"	// Here's a helpful diagram to keep in mind while trying to\n"
	"	// understand the code:\n"
	"	//\n"
	"	//  |      |      |      |      |\n"
	"	// -------------------------------\n"
	"	//  |      |      |      |      |\n"
	"	//  |  01  |  11  |  21  |  31  | <-- current scanline\n"
	"	//  |      | @    |      |      |\n"
	"	// -------------------------------\n"
	"	//  |      |      |      |      |\n"
	"	//  |  02  |  12  |  22  |  32  | <-- next scanline\n"
	"	//  |      |      |      |      |\n"
	"	// -------------------------------\n"
	"	//  |      |      |      |      |\n"
	"	//\n"
	"	// Each character-cell represents a pixel on the output\n"
	"	// surface, '@' represents the current pixel (always somewhere\n"
	"	// in the bottom half of the current scan-line, or the top-half\n"
	"	// of the next scanline). The grid of lines represents the\n"
	"	// edges of the texels of the underlying texture.\n"

	"	// Texture coordinates of the texel containing the active pixel.\n"
	"	if (param == 0.0) {\n"
	"		// Enable screen curvature.\n"
	"		xy = transform(texCoord);\n"
	"	} else {\n"
	"		xy = texCoord;\n"
	"	}\n"

	"	float cval = corner(xy);\n"

	"	// Of all the pixels that are mapped onto the texel we are\n"
	"	// currently rendering, which pixel are we currently rendering?\n"
	"	vec2 ratio_scale = (xy * 256.0) + 0.5;\n"

	"	vec2 uv_ratio = fract(ratio_scale);\n"

	"	// Snap to the center of the underlying texel.\n"
	"	xy = (floor(ratio_scale) - 0.5) / 256.0;\n"

	"	// Calculate Lanczos scaling coefficients describing the effect\n"
	"	// of various neighbour texels in a scanline on the current\n"
	"	// pixel.\n"
	"	vec4 coeffs = PI * vec4(1.0 + uv_ratio.x, uv_ratio.x, 1.0 - uv_ratio.x, 2.0 - uv_ratio.x);\n"

	"	// Prevent division by zero.\n"
	"	coeffs = FIX(coeffs);\n"

	"	// Lanczos2 kernel.\n"
	"	coeffs = 2.0 * sin(coeffs) * sin(coeffs / 2.0) / (coeffs * coeffs);\n"

	"	// Normalize.\n"
	"	coeffs /= dot(coeffs, vec4(1.0));\n"

	"	// Calculate the effective colour of the current and next\n"
	"	// scanlines at the horizontal location of the current pixel,\n"
	"	// using the Lanczos coefficients above.\n"
	"	if (full_interpolation == 1.0) {\n"
	"		// LINEAR_PROCESSING ON (slow)\n"
	"		col = clamp(mat4("
	"				pow(texture2D(texture_scr, xy + vec2(-one.x, 0.0)), vec4(CRTgamma)),"
	"				pow(texture2D(texture_scr, xy), vec4(CRTgamma)),"
	"				pow(texture2D(texture_scr, xy + vec2(one.x, 0.0)), vec4(CRTgamma)),"
	"				pow(texture2D(texture_scr, xy + vec2(2.0 * one.x, 0.0)), vec4(CRTgamma))) * coeffs,"
	"				0.0, 1.0);\n"
	"		col2 = clamp(mat4("
	"				pow(texture2D(texture_scr, xy + vec2(-one.x, one.y)), vec4(CRTgamma)),"
	"				pow(texture2D(texture_scr, xy + vec2(0.0, one.y)), vec4(CRTgamma)),"
	"				pow(texture2D(texture_scr, xy + one), vec4(CRTgamma)),"
	"				pow(texture2D(texture_scr, xy + vec2(2.0 * one.x, one.y)), vec4(CRTgamma))) * coeffs,"
	"				0.0, 1.0);\n"
	"	} else {\n"
	"		// LINEAR_PROCESSING OFF (gain speed)\n"
	"		col = clamp(mat4("
	"				texture2D(texture_scr, xy + vec2(-one.x, 0.0)),"
	"				texture2D(texture_scr, xy),"
	"				texture2D(texture_scr, xy + vec2(one.x, 0.0)),"
	"				texture2D(texture_scr, xy + vec2(2.0 * one.x, 0.0))) * coeffs,"
	"				0.0, 1.0);\n"
	"		col2 = clamp(mat4("
	"				texture2D(texture_scr, xy + vec2(-one.x, one.y)),"
	"				texture2D(texture_scr, xy + vec2(0.0, one.y)),"
	"				texture2D(texture_scr, xy + one),"
	"				texture2D(texture_scr, xy + vec2(2.0 * one.x, one.y))) * coeffs,"
	"				0.0, 1.0);\n"
	"		col = pow(col , vec4(CRTgamma));\n"
	"		col2 = pow(col2, vec4(CRTgamma));\n"
	"	}\n"

	"	// Calculate the influence of the current and next scanlines on\n"
	"	// the current pixel.\n"
	"	vec4 weights = scanlineWeights(uv_ratio.y, col);\n"
	"	vec4 weights2 = scanlineWeights(1.0 - uv_ratio.y, col2);\n"

	"// Enable 3x oversampling of the beam profile\n"
	"	if (param == 0.0) {\n"
	"		float filter = fwidth(ratio_scale.y);\n"
	"		uv_ratio.y =uv_ratio.y+1.0/3.0*filter;\n"
	"		weights = (weights+scanlineWeights(uv_ratio.y, col))/3.0;\n"
	"		weights2=(weights2+scanlineWeights(abs(1.0-uv_ratio.y), col2))/3.0;\n"
	"		uv_ratio.y =uv_ratio.y-2.0/3.0*filter;\n"
	"		weights=weights+scanlineWeights(abs(uv_ratio.y), col)/3.0;\n"
	"		weights2=weights2+scanlineWeights(abs(1.0-uv_ratio.y), col2)/3.0;\n"
	"	}\n"

	"	vec3 mul_res = (col * weights + col2 * weights2).rgb * cval;\n"

	"	// dot-mask emulation:\n"
	"	// Output pixels are alternately tinted green and magenta.\n"
	"	vec3 dotMaskWeights = mix("
	"			vec3(1.0, 0.7, 1.0),"
	"			vec3(0.7, 1.0, 0.7),"
	"			floor(mod(mod_factor, 2.0))"
	"	);\n"

	"	mul_res *= dotMaskWeights;\n"

	"	// Convert the image gamma for display on our output device.\n"
	"	mul_res = pow(mul_res, vec3(1.0 / monitorgamma));\n"

	"	// Color the texel.\n"
	"	vec4 scr = vec4(mul_res, 1.0);\n"

	"	gl_FragColor = scr * gl_Color;\n"
	"}"
},
