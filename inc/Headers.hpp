/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Headers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajuliao- <ajuliao-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:30:49 by mchamma           #+#    #+#             */
/*   Updated: 2025/04/09 20:22:47 by ajuliao-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADERS_HPP
#define HEADERS_HPP

#include <iostream>		// std
#include <cstring>		// memset()
#include <cstdio>		// stdin
#include <vector>		// container: vector
#include <map>			// container: map
#include <csignal>		// signal
#include <algorithm>	// atoi()
#include <sstream>		// stringstream
#include <iomanip>		// setw()
#include <unistd.h> 	// close()
#include <poll.h>		// pool()
#include <arpa/inet.h>	// inet_ntoa()
#include <fcntl.h>		// fcnl()

//-------------------------------------------------------//

// #ifdef USE_IRC_COLOR

// #define RED  std::string("\x03") + "04"
// #define YEL  std::string("\x03") + "08"
// #define GRE  std::string("\x03") + "03"
// #define BLU  std::string("\x03") + "12"
// #define WHI  std::string("\x03") + "00"

// #else

// #define RED  std::string("\e[1;31m")  // Red
// #define YEL  std::string("\e[1;33m")  // Yellow
// #define GRE  std::string("\e[1;32m")  // Green
// #define BLU  std::string("\e[1;34m")  // Blue
// #define WHI  std::string("\e[0;37m")  // White

// #endif

//-------------------------------------------------------//

#define TIME getCurrentLocalTime()+ " - " // current local time

//-------------------------------------------------------//

#endif
