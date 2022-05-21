<!--
	Copyright 2022 fm1619

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
-->

# Max Clique Problem Solver using Branch and Bound method

***mcp-solver*** is a **Max Clique Problem Solver** using the **Branch and Bound** method.

Input files are in the **DIMACS** file format. More about it [here](http://prolland.free.fr/works/research/dsat/dimacs.html).

# Compilation :
`make`

## Execution :
`./mcps [file.clq]`
> Samples files are available in `samples/`

## Example of use:

`make exemple`

`./mcps samples/gen200_p09_55.clq`

`./mcps samples/hamming10-2.clq`
