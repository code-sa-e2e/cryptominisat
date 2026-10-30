#include <cstdio>
#include <iostream>
#include <vector>
#include <cryptominisat5/cryptominisat.h>

// API sequence: CMSat::SATSolver::set_verbosity(unsigned int) -> CMSat::SATSolver::new_var() -> CMSat::SATSolver::okay() const
int Test_newVar() {
  printf("[DEBUG] Test_newVar: START\n");

  printf("[DEBUG] Creating CMSat::SATSolver object...\n");
  CMSat::SATSolver solver;
  CMSat::SATSolver* s = &solver;
  printf("Solver created\n");
  printf("[DEBUG] Solver pointer s = %p\n", (void*)s);

  printf("[DEBUG] Calling set_verbosity(0)...\n");
  s->set_verbosity(0);
  printf("set_verbosity(0) called\n");
  printf("[DEBUG] set_verbosity(0) returned\n");

  printf("[DEBUG] Calling nVars() to get initial count...\n");
  unsigned initialVars = s->nVars();
  printf("[DEBUG] nVars() returned %u\n", initialVars);
  printf("nVars before new_var: %u\n", initialVars);
  if (initialVars != 0) {
    printf("[DEBUG] Branch taken: initialVars != 0 (value=%u)\n", initialVars);
    printf("Unexpected initial variable count\n");
    printf("[DEBUG] Test_newVar: returning -1 (initial var count check)\n");
    return -1;
  } else {
    printf("[DEBUG] Branch not taken: initialVars == 0, as expected\n");
  }

  // new_var() returns void, so there is no return value to capture;
  // its effect is validated through nVars() and okay().
  printf("[DEBUG] Calling new_var() (1st)...\n");
  s->new_var();
  printf("[DEBUG] new_var() (1st) returned\n");
  printf("new_var() executed successfully (void return, no value to output)\n");

  printf("[DEBUG] Calling nVars() after 1st new_var...\n");
  unsigned n = s->nVars();
  printf("[DEBUG] nVars() returned %u\n", n);
  printf("nVars after new_var: %u\n", n);
  std::cout << "nVars: " << n << std::endl;

  if (n != 1) {
    printf("[DEBUG] Branch taken: n != 1 (n=%u)\n", n);
    printf("Test failed: expected 1 variable after one new_var(), got %u\n", n);
    printf("[DEBUG] Test_newVar: returning -1 (after 1st new_var check)\n");
    return -1;
  } else {
    printf("[DEBUG] Branch not taken: n == 1, as expected\n");
  }

  printf("[DEBUG] Calling okay() after 1st new_var...\n");
  bool ok = s->okay();
  printf("[DEBUG] okay() returned %d\n", (int)ok);
  printf("okay() returned: %d\n", (int)ok);
  if (!ok) {
    printf("[DEBUG] Branch taken: !ok\n");
    printf("Solver is not in okay state after new_var()\n");
    printf("[DEBUG] Test_newVar: returning -1 (okay check 1)\n");
    return -1;
  } else {
    printf("[DEBUG] Branch not taken: solver is okay\n");
  }

  // Call new_var() again and verify the count increments by exactly one each time
  printf("[DEBUG] Calling new_var() (2nd)...\n");
  s->new_var();
  printf("[DEBUG] new_var() (2nd) returned\n");
  printf("second new_var() executed successfully (void return, no value to output)\n");
  printf("[DEBUG] Calling nVars() after 2nd new_var...\n");
  unsigned n2 = s->nVars();
  printf("[DEBUG] nVars() returned %u\n", n2);
  printf("nVars after second new_var: %u\n", n2);
  if (n2 != 2) {
    printf("[DEBUG] Branch taken: n2 != 2 (n2=%u)\n", n2);
    printf("Test failed: expected 2 variables after two new_var() calls, got %u\n", n2);
    printf("[DEBUG] Test_newVar: returning -1 (after 2nd new_var check)\n");
    return -1;
  } else {
    printf("[DEBUG] Branch not taken: n2 == 2, as expected\n");
  }

  printf("[DEBUG] Calling new_var() (3rd)...\n");
  s->new_var();
  printf("[DEBUG] new_var() (3rd) returned\n");
  printf("third new_var() executed successfully (void return, no value to output)\n");
  printf("[DEBUG] Calling nVars() after 3rd new_var...\n");
  unsigned n3 = s->nVars();
  printf("[DEBUG] nVars() returned %u\n", n3);
  printf("nVars after third new_var: %u\n", n3);
  if (n3 != 3) {
    printf("[DEBUG] Branch taken: n3 != 3 (n3=%u)\n", n3);
    printf("Test failed: expected 3 variables after three new_var() calls, got %u\n", n3);
    printf("[DEBUG] Test_newVar: returning -1 (after 3rd new_var check)\n");
    return -1;
  } else {
    printf("[DEBUG] Branch not taken: n3 == 3, as expected\n");
  }

  printf("[DEBUG] Calling okay() after 3 new_var calls...\n");
  bool ok2 = s->okay();
  printf("[DEBUG] okay() returned %d\n", (int)ok2);
  if (!ok2) {
    printf("[DEBUG] Branch taken: !okay()\n");
    printf("Solver is not in okay state after multiple new_var() calls\n");
    printf("[DEBUG] Test_newVar: returning -1 (okay check 2)\n");
    return -1;
  } else {
    printf("[DEBUG] Branch not taken: solver is okay after multiple new_var() calls\n");
  }

  // Verify the newly created variables are usable: a unit clause on variable 0 must be satisfiable
  printf("[DEBUG] Building clause with Lit(0, false)...\n");
  std::vector<CMSat::Lit> clause;
  clause.push_back(CMSat::Lit(0, false));
  printf("[DEBUG] clause built, size = %zu\n", clause.size());
  printf("[DEBUG] Calling add_clause(clause)...\n");
  s->add_clause(clause);
  printf("[DEBUG] add_clause returned\n");
  printf("[DEBUG] Calling solve()...\n");
  CMSat::lbool res = s->solve();
  printf("[DEBUG] solve() returned\n");
  printf("[DEBUG] res == l_True: %d, res == l_False: %d, res == l_Undef: %d\n",
         (int)(res == CMSat::l_True), (int)(res == CMSat::l_False), (int)(res == CMSat::l_Undef));
  printf("solve() on created variables returned: %d\n", (int)(res == CMSat::l_True));
  if (res != CMSat::l_True) {
    printf("[DEBUG] Branch taken: res != l_True\n");
    printf("Test failed: expected satisfiable result using new variables\n");
    printf("[DEBUG] Test_newVar: returning -1 (solve result check)\n");
    return -1;
  } else {
    printf("[DEBUG] Branch not taken: res == l_True, as expected\n");
  }

  printf("[DEBUG] Calling get_model()...\n");
  const std::vector<CMSat::lbool>& model = s->get_model();
  printf("[DEBUG] get_model() returned, model.size() = %zu\n", model.size());
  for (size_t i = 0; i < model.size(); i++) {
    printf("[DEBUG] model[%zu]: l_True=%d l_False=%d l_Undef=%d\n", i,
           (int)(model[i] == CMSat::l_True),
           (int)(model[i] == CMSat::l_False),
           (int)(model[i] == CMSat::l_Undef));
  }
  if (model.size() != 3) {
    printf("[DEBUG] Branch taken: model.size() != 3\n");
    printf("Test failed: model size expected 3, got %zu\n", model.size());
    printf("[DEBUG] Test_newVar: returning -1 (model size check)\n");
    return -1;
  } else {
    printf("[DEBUG] Branch not taken: model.size() == 3, as expected\n");
  }
  if (model[0] != CMSat::l_True) {
    printf("[DEBUG] Branch taken: model[0] != l_True\n");
    printf("Test failed: variable 0 should be true in model\n");
    printf("[DEBUG] Test_newVar: returning -1 (model[0] check)\n");
    return -1;
  } else {
    printf("[DEBUG] Branch not taken: model[0] == l_True, as expected\n");
  }

  printf("[DEBUG] Calling nVars() after solve...\n");
  unsigned n4 = s->nVars();
  printf("[DEBUG] nVars() returned %u\n", n4);
  if (n4 != 3) {
    printf("[DEBUG] Branch taken: nVars() != 3 (value=%u)\n", n4);
    printf("Test failed: nVars changed unexpectedly after solve\n");
    printf("[DEBUG] Test_newVar: returning -1 (nVars after solve check)\n");
    return -1;
  } else {
    printf("[DEBUG] Branch not taken: nVars() == 3 after solve, as expected\n");
  }

  printf("Test passed\n");
  printf("[DEBUG] Test_newVar: END (returning 0)\n");
  return 0;
}

int main() {
  setvbuf(stdout, NULL, _IONBF, 0);  // unbuffered so output is visible even on crash
  printf("[DEBUG] main: START\n");
  int rc = Test_newVar();
  printf("[DEBUG] main: Test_newVar returned %d\n", rc);
  printf("[DEBUG] main: END (%s)\n", rc == 0 ? "SUCCESS" : "FAILURE");
  return rc;
}