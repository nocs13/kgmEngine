#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../../kgmMath/kgmMath.h"

#define toGrad(x) x * 180.0 / 3.14156
void usage()
{

}

int main(int argc, char** argv)
{
  quat q;

  if(argc < 2)
    {
      usage();

      return 1;
    }

  if(!strcmp(argv[1], "quat_to_euler"))
    {
      vec3 vr;

      q = quat(atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]));
      mtx4 m(q);
      m.angles(vr);

      printf("euler is %f %f %f\n", toGrad(vr.x), toGrad(vr.y), toGrad(vr.z));
    }

  return 0;
}
