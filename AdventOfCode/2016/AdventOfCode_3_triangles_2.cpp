/*
 Description:   Now that you can think clearly, you move deeper into the
                labyrinth of hallways and office furniture that makes up this
                part of Easter Bunny HQ. This must be a graphic design
                department; the walls are covered in specifications for
                triangles. Or are they?
 
                The design document gives the side lengths of each triangle it
                describes, but... 5 10 25? Some of these aren't triangles. You
                can't help but mark the impossible ones.
 
                In a valid triangle, the sum of any two sides must be larger
                than the remaining side. For example, the "triangle" given above
                is impossible, because 5 + 10 is not larger than 25.
                 
                In your puzzle input, how many of the listed triangles are
                possible?

                Now that you've helpfully marked up their design documents, it
                occurs to you that triangles are specified in groups of three
                vertically. Each set of three numbers in a column specifies a
                triangle. Rows are unrelated.

                For example, given the following specification, numbers with the
                same hundreds digit would be part of the same triangle:

                101 301 501
                102 302 502
                103 303 503
                201 401 601
                202 402 602
                203 403 603
                
                In your puzzle input, and instead reading by columns, how many
                of the listed triangles are possible?
                
  Programmer:   Michael Duarte.

  Date:         12/03/2016
*/

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    vector<vector<int>> vec(3,vector<int>(3));
    int counter = 0;
    int line = -1;
    while(!std::cin.eof()) {
        ++line;
        cin >> vec[0][line] >> vec[1][line] >> vec[2][line];
        
        if (line == 2){
            sort(vec[0].begin(), vec[0].end());
            sort(vec[1].begin(), vec[1].end());
            sort(vec[2].begin(), vec[2].end());
            for (int i = 0 ; i < 3; ++i) {
                if (vec[i][0] + vec[i][1] > vec[i][2])
                    ++counter;
                cout << vec[i][0] << ", " << vec[i][1] << ", " << vec[i][2];
                cout << " = " << counter << endl;
            }
            line = -1;
        }
    }
    cout << counter << endl;
    
    return EXIT_SUCCESS;
}
