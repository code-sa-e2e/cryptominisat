/******************************************
Copyright (c) 2016, @Storyyeller

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
***********************************************/


#include "cryptominisat5/cryptominisat_c.h"
#include "assert.h"

c_Lit new_lit(uint32_t var, bool neg) {
    c_Lit x;
    x.x = (var << 1) | neg;
    return x;
}

int main(void) {
    int new; // make sure this is actually compiled as C

    SATSolver *solver = cmsat_new();
    cmsat_set_num_threads(solver, 4);
    cmsat_new_vars(solver, 3);

    c_Lit clause[4];
    clause[0] = new_lit(0, false);
    cmsat_add_clause(solver, clause, 1);

    clause[0] = new_lit(1, true);
    cmsat_add_clause(solver, clause, 1);

    clause[0] = new_lit(0, true);
    clause[1] = new_lit(1, false);
    clause[2] = new_lit(2, false);
    cmsat_add_clause(solver, clause, 3);

    c_lbool ret = cmsat_solve(solver);
    assert(ret.x == L_TRUE);

    slice_lbool model = cmsat_get_model(solver);
    assert(model.vals[0].x == L_TRUE);
    assert(model.vals[1].x == L_FALSE);
    assert(model.vals[2].x == L_TRUE);
    // solving with an added conflicting unit clause gives UNSAT
    clause[0] = new_lit(2, true);
    cmsat_add_clause(solver, clause, 1);
    ret = cmsat_solve(solver);
    assert(ret.x == L_FALSE);
    // solving again stays UNSAT
    ret = cmsat_solve(solver);
    assert(ret.x == L_FALSE);
    cmsat_free(solver);

    // contradictory unit clauses on a single variable
    solver = cmsat_new();
    cmsat_set_verbosity(solver, 0);
    cmsat_new_vars(solver, 1);
    clause[0] = new_lit(0, false);
    cmsat_add_clause(solver, clause, 1);
    clause[0] = new_lit(0, true);
    cmsat_add_clause(solver, clause, 1);
    ret = cmsat_solve(solver);
    assert(ret.x == L_FALSE);
    cmsat_free(solver);

    // simple satisfiable instance with 2 threads
    solver = cmsat_new();
    cmsat_set_verbosity(solver, 0);
    cmsat_set_num_threads(solver, 2);
    cmsat_new_vars(solver, 4);
    clause[0] = new_lit(0, false);
    clause[1] = new_lit(1, false);
    cmsat_add_clause(solver, clause, 2);
    clause[0] = new_lit(2, false);
    clause[1] = new_lit(3, true);
    cmsat_add_clause(solver, clause, 2);
    ret = cmsat_solve(solver);
    assert(ret.x == L_TRUE);
    cmsat_free(solver);
    // set_no_bva on fresh solver, then solve a SAT instance
    {
        c_Lit clause[4];
        solver = cmsat_new();
        cmsat_set_verbosity(solver, 0);
        cmsat_set_no_bva(solver);
        cmsat_new_vars(solver, 3);

        clause[0] = new_lit(0, false);
        cmsat_add_clause(solver, clause, 1);
        clause[0] = new_lit(1, true);
        cmsat_add_clause(solver, clause, 1);
        clause[0] = new_lit(0, true);
        clause[1] = new_lit(1, false);
        clause[2] = new_lit(2, false);
        cmsat_add_clause(solver, clause, 3);

        ret = cmsat_solve(solver);
        assert(ret.x == L_TRUE);
        slice_lbool model = cmsat_get_model(solver);
        assert(model.num_vals == 3);
        assert(model.vals[0].x == L_TRUE);
        assert(model.vals[1].x == L_FALSE);
        assert(model.vals[2].x == L_TRUE);
        cmsat_free(solver);

        // calling it repeatedly is harmless; instance with many equal-structure clauses (BVA candidates)
        solver = cmsat_new();
        cmsat_set_verbosity(solver, 0);
        cmsat_set_no_bva(solver);
        cmsat_set_no_bva(solver);
        cmsat_new_vars(solver, 12);
        for (uint32_t a = 0; a < 4; a++) {
            for (uint32_t b = 4; b < 8; b++) {
                clause[0] = new_lit(a, false);
                clause[1] = new_lit(b, false);
                clause[2] = new_lit(8, false);
                cmsat_add_clause(solver, clause, 3);
            }
        }
        clause[0] = new_lit(8, true);
        clause[1] = new_lit(9, false);
        cmsat_add_clause(solver, clause, 2);
        ret = cmsat_solve(solver);
        assert(ret.x == L_TRUE);

        // make it UNSAT
        clause[0] = new_lit(9, true);
        cmsat_add_clause(solver, clause, 1);
        clause[0] = new_lit(8, false);
        cmsat_add_clause(solver, clause, 1);
        ret = cmsat_solve(solver);
        assert(ret.x == L_FALSE);
        cmsat_free(solver);
    }

}
