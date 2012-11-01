/*
 * 3-Clause BSD License
 *
 * Copyright (c) 2012, Sven Bachmann <dev@mcbachmann.de>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "utf8_helper.h"


/**
 * Pad or truncate an utf8 string to specified len.
 *
 * Normal string functions like printf count utf-8 by bytes instead of visible
 * characters. This function truncates or pads the string to the given length
 * \sa len by adding spaces with the formula [pad = length - visible length].
 *
 * If the parameter \sa free_src_str is set to true the old string will be
 * freed if a new string must be allocated or if \sa always_dup is set.
 *
 * The parameter \sa always_dup is set, a new string is always allocated, even
 * if the length is already at the desired value.
 *
 * WARNING: Error handling is currently not implemented - every error results
 *          in an assert.
 *
 * \param str           string to add space padding
 * \param len           desired string length
 * \param free_src_str  set to true if source string should be freed
 * \param always_dup    set to true if returned string should always be allocated
 *
 * \returns pointer to new or old string, see description for details 
 */
char* utf8_resize(char *str, int len, bool free_src_str, bool always_dup)
{
    int pad;
    int vislen;
    int new_strlen;
    int old_strlen;
    char *newstr;
    
    // count string length with regards to utf8 characters
    vislen = mbstowcs(NULL, str, 0);
    assert(0 <= vislen);

    // check if string should be truncated
    if (len < vislen) {

	// reserve stack memory for len wide chars + null byte
	wchar_t dst_wc[len + 1];

	// convert string to wide char
	vislen = mbstowcs(dst_wc, str, len);
	assert(0 <= vislen);

	// add null terminator at string end
	dst_wc[len] = 0;

	// determine size of multibyte string
	vislen = wcstombs(NULL, dst_wc, 0);
	assert(0 <= vislen);

	// allocate multibyte string + terminator byte
	newstr = malloc(vislen + 1);
	assert(NULL != newstr);

	// convert wide char string back to byte string
	vislen = wcstombs(newstr, dst_wc, vislen + 1);
	assert(0 <= vislen);

	// check if old string should be freed
	if (true == free_src_str) {
	    free(str);
	}

	return (newstr);
    }

    // check if visible length is too small or already at len
    if (len == vislen) {
	if (true == always_dup) {
	    return (strdup(str));
	}
	return (str);
    }

    // allocate new string + padding length + 1 termination byte
    pad = len - vislen;
    old_strlen = strlen(str);
    new_strlen = old_strlen + pad;
    newstr = malloc(new_strlen + 1);
    assert(NULL != newstr);

    // copy old string to new space
    assert(newstr == memcpy(newstr, str, old_strlen));

    // release old string
    if (true == free_src_str) {
	free(str);
    }

    // pad new string with pad-bytes
    newstr[old_strlen + pad] = 0;
    while (pad) {
	newstr[old_strlen + --pad] = ' ';
    }

    return (newstr);
}

