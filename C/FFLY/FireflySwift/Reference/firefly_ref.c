/*------------------------------------------------------------------------
  firefly_ref.c  -  portable C reference of the 1993 FIREFLY.C core

  This is a literal port of CalcFlash() and of the cell state machine in
  DrawFlies() from ../../FIREFLY.C (Ed Nieuwenhuys, 1993) with all GDI /
  Win16 code removed and a seedable random generator added.  It exists to
  generate the "golden" numbers that FireflyEngineTests (Swift) compare
  against, so the Swift port is checked against the original algorithm.

  Differences from FIREFLY.C (on purpose):
    * no window / drawing: after every tick we print what DrawFlies would
      show (level 2 = bright, 1 = dim/glow, 0 = dark)
    * random(threshold) is replaced by SplitMix64 % threshold, so that the
      Swift port can reproduce the very same start state
    * the original ignores the "Energie" setting (base charge is fixed 10);
      so does this reference.  The Swift port uses charge = 10*Energie/110,
      which is identical at the default Energie of 110.

  Build:  cc -O2 -o firefly_ref firefly_ref.c
  Usage:  firefly_ref cols rows cellaround thresholddiv glow ticks seed
  Output: one line per tick
          tick bright dim levelHash energyHash
          (hashes: FNV-1a 64 over level bytes / over int32 energies, LE)
  ------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define THRESHOLD 1000

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

static int  cellaround = 4;
static int  thresholddiv = 4;
static int  Glow = 1;
static int  nooffliesx = 120;
static int  nooffliesy = 100;

static long *energie;
static long *genergie;
static char *flash;
static unsigned char *level;   /* what DrawFlies would paint */

/* ---- SplitMix64 (same constants in the Swift port) ------------------- */
static uint64_t sm_state;
static uint64_t sm_next(void)
{
    uint64_t z = (sm_state += 0x9E3779B97F4A7C15ULL);
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
}

/* ---- Init(): energies random(threshold); x outer loop, y inner ------- */
static void init(void)
{
    int m, n;
    for (n = 0; n < nooffliesx; n++)
        for (m = 0; m < nooffliesy; m++)
        {
            energie[n + nooffliesx * m] = (long)(sm_next() % THRESHOLD);
            flash[n + nooffliesx * m] = 0;
            level[n + nooffliesx * m] = 0;
        }
}

/* ---- CalcFlash(): verbatim from FIREFLY.C ---------------------------- */
static void calc_flash(void)
{
    int m, n, i, j, u, k, l, km, lm;
    int distance, maxdistance;

    maxdistance = 2 * (cellaround) * (cellaround);
    for (n = 0; n < nooffliesx; n++)
        for (m = 0; m < nooffliesy; m++)
        {
            u = n + nooffliesx * m;
            if (energie[u] >= THRESHOLD)
            {
                k = max(0, n - cellaround);
                l = max(0, m - cellaround);
                km = min(nooffliesx - 1, n + cellaround);
                lm = min(nooffliesy - 1, m + cellaround);
                for (i = k; i <= km; i++)
                    for (j = l; j <= lm; j++)
                    {
                        if (i == n && j == m) continue;
                        distance = (i - n) * (i - n) + (j - m) * (j - m);
                        genergie[i + nooffliesx * j] +=
                            (int)(0.5 + 0.5 * ((float)(THRESHOLD) *
                                  ((float)maxdistance / (float)distance / (float)maxdistance)))
                            / thresholddiv;
                    }
                flash[u] = 2;
            }
            else genergie[u] += 10;
        }
    for (n = 0; n < nooffliesx; n++)
        for (m = 0; m < nooffliesy; m++)
        {
            energie[n + nooffliesx * m] += genergie[n + nooffliesx * m];
            genergie[n + nooffliesx * m] = 0;
            if (flash[n + nooffliesx * m] == 2) energie[n + nooffliesx * m] = 0;
        }
}

/* ---- DrawFlies(): only the state machine (same sequential ifs) ------- */
static void draw_flies(void)
{
    int m, n, u;
    for (m = 0; m < nooffliesy; m++)
        for (n = 0; n < nooffliesx; n++)
        {
            u = n + nooffliesx * m;
            if (flash[u] == 0) level[u] = 0;                       /* black   */
            if (flash[u] == 1) { flash[u] = 0;    level[u] = 1; }  /* dim     */
            if (flash[u] == 2) { flash[u] = Glow; level[u] = 2; }  /* bright  */
        }
}

static uint64_t fnv_bytes(uint64_t h, const unsigned char *p, size_t n)
{
    size_t i;
    for (i = 0; i < n; i++) { h ^= p[i]; h *= 0x100000001B3ULL; }
    return h;
}

int main(int argc, char **argv)
{
    int ticks = 100, t, u, total;
    uint64_t seed = 1;

    if (argc > 1) nooffliesx   = atoi(argv[1]);
    if (argc > 2) nooffliesy   = atoi(argv[2]);
    if (argc > 3) cellaround   = atoi(argv[3]);
    if (argc > 4) thresholddiv = atoi(argv[4]);
    if (argc > 5) Glow         = atoi(argv[5]) ? 1 : 0;
    if (argc > 6) ticks        = atoi(argv[6]);
    if (argc > 7) seed         = strtoull(argv[7], NULL, 10);

    total    = nooffliesx * nooffliesy;
    energie  = calloc(total, sizeof(long));
    genergie = calloc(total, sizeof(long));
    flash    = calloc(total, 1);
    level    = calloc(total, 1);
    sm_state = seed;
    init();

    for (t = 1; t <= ticks; t++)
    {
        int bright = 0, dim = 0;
        uint64_t lh = 0xCBF29CE484222325ULL, eh = 0xCBF29CE484222325ULL;

        calc_flash();
        draw_flies();
        for (u = 0; u < total; u++)
        {
            unsigned char e[4];
            uint32_t v = (uint32_t)(int32_t)energie[u];
            if (level[u] == 2) bright++;
            if (level[u] == 1) dim++;
            e[0] = v & 255; e[1] = (v >> 8) & 255; e[2] = (v >> 16) & 255; e[3] = (v >> 24) & 255;
            eh = fnv_bytes(eh, e, 4);
        }
        lh = fnv_bytes(lh, level, (size_t)total);
        printf("%d %d %d %016llx %016llx\n", t, bright, dim,
               (unsigned long long)lh, (unsigned long long)eh);
    }
    return 0;
}
