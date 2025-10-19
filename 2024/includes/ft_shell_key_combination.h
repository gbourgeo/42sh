/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_shell_key_combination.h                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbourgeo <gbourgeo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/12/28 04:49:32 by gbourgeo          #+#    #+#             */
/*   Updated: 2018/04/06 17:34:07 by gbourgeo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _FT_SHELL_KEY_COMBINATION_H_
#define _FT_SHELL_KEY_COMBINATION_H_

/*      Nom de la combinaison   Valeur,   Longueur */
#define KEYBOARD_CTRL_C         { 0x03 }, 1
#define KEYBOARD_CTRL_D         { 0x04 }, 1
#define KEYBOARD_CTRL_BACKSPACE { 0x08 }, 1
#define KEYBOARD_CTRL_L         { 0x0C }, 1
#define KEYBOARD_CTRL_P         { 0x10 }, 1
#define KEYBOARD_CTRL_Y         { 0x19 }, 1
#define KEYBOARD_CTRL_UP        { 0x1B, 0x5B, 0x31, 0x3B, 0x35, 0x41 }, 6
#define KEYBOARD_CTRL_DOWN      { 0x1B, 0x5B, 0x31, 0x3B, 0x35, 0x42 }, 6
#define KEYBOARD_CTRL_RIGHT     { 0x1B, 0x5B, 0x31, 0x3B, 0x35, 0x43 }, 6
#define KEYBOARD_CTRL_LEFT      { 0x1B, 0x5B, 0x31, 0x3B, 0x35, 0x44 }, 6
#define KEYBOARD_CTRL_DELETE    { 0x1B, 0x5b, 0x33, 0x3b, 0x35, 0x7e }, 6 // under Apple, but Linux is ???

#define KEYBOARD_ALT_ENTER      { 0x1B, 0x0A }, 2                         // under Apple, but Linux is ???
#define KEYBOARD_ALT_UP         { 0x1B, 0x5B, 0x31, 0x3B, 0x33, 0x41 }, 6
#define KEYBOARD_ALT_DOWN       { 0x1B, 0x5B, 0x31, 0x3B, 0x33, 0x42 }, 6
#define KEYBOARD_ALT_DELETE     { 0x1B, 0x5B, 0x33, 0x3B, 0x33, 0x7E }, 6
#define KEYBOARD_ALT_H          { 0x1B, 0x68 }, 2
#define KEYBOARD_ALT_P          { 0x1B, 0x70 }, 2
#define KEYBOARD_ALT_Y          { 0x1B, 0x79 }, 2

#if defined(__APPLE__) || defined(__MACH__)
# define KEYBOARD_ALT_BACKSPACE { 0x17 }, 1
# define KEYBOARD_ALT_LEFT      { 0x1B, 0x62 }, 2
# define KEYBOARD_ALT_RIGHT     { 0x1B, 0x66 }, 2
#else
# define KEYBOARD_ALT_BACKSPACE { 0x1B, 0x7F }, 2
# define KEYBOARD_ALT_LEFT      { 0x1B, 0x5B, 0x31, 0x3B, 0x33, 0x44 }, 6
# define KEYBOARD_ALT_RIGHT     { 0x1B, 0x5B, 0x31, 0x3B, 0x33, 0x43 }, 6
#endif

#define KEYBOARD_ENTER     { 0x0A }, 1
#define KEYBOARD_BACKSPACE { 0x7F }, 1
#define KEYBOARD_DELETE    { 0x1B, 0x5B, 0x33, 0x7E }, 4
#define KEYBOARD_UP        { 0x1B, 0x5B, 0x41 }, 3
#define KEYBOARD_DOWN      { 0x1B, 0x5B, 0x42 }, 3
#define KEYBOARD_RIGHT     { 0x1B, 0x5B, 0x43 }, 3
#define KEYBOARD_LEFT      { 0x1B, 0x5B, 0x44 }, 3
#define KEYBOARD_END       { 0x1B, 0x5B, 0x46 }, 3
#define KEYBOARD_HOME      { 0x1B, 0x5B, 0x48 }, 3
#define KEYBOARD_ESC       { 0x1B }, 1 // under Apple, but Linux is ???

#define KEYBOARD_

#endif /* _FT_SHELL_KEY_COMBINATION_H_ */
