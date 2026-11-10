#ifndef FASTYMD_LEAPYEAR_H
#define FASTYMD_LEAPYEAR_H

/*
 * From stackoverflow: https://stackoverflow.com/a/79564914
 * user: https://stackoverflow.com/users/17321211/jerichaux
 * License: CC BY-SA 4.0 (https://creativecommons.org/licenses/by-sa/4.0/)
 */
#define ISLEAP(yearNum) (!(((yearNum) & 3) | ((yearNum) & (((16 - (!((yearNum) % 25))) | 16) ^ 16))))

#endif
