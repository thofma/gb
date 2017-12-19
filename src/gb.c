/* gb: Gröbner Basis
 * Copyright (C) 2015 Christian Eder <ederc@mathematik.uni-kl.de>
 * This file is part of gb.
 * gbla is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * gbla is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with gbla . If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file gb.c
 * \brief Overall GB library file
 *
 * \author Christian Eder <ederc@mathematik.uni-kl.de>
 */

#include <stdio.h>
#include <stdlib.h>

#include "stat.c" /* computational statistics */
#include "time.c" /* timing business */

#include "data.c" /* general data types */
#include "io.c"   /* input and output data handling */
