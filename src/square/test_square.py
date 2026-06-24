# To run tests: python3 -m unittest -v test_square

import subprocess
import unittest

exec_name = "square.o"

def dsquare(x):
    return 2*x

class SquareTest(unittest.TestCase):
    def test_dsquare_dx(self):
        """
        Test d(square)/dx for multiple values
        """
        test_vals =  [-1.0, 0.0, 1.5, 3.2, 27.876]
        
        for k, test_val in enumerate(test_vals):
            with self.subTest(f"x ={test_val}", i=k):
                run_result = subprocess.run([f"./{exec_name}", f"{test_val}"], capture_output=True)
                dsquare_dx_enzyme = float(run_result.stdout.strip())
                dsquare_dx_expected = dsquare(test_val)
            
                print(f"(i={k}) x = {test_val}; d(square)/dx Enzyme = {dsquare_dx_enzyme}; d(square)/dx Expected = {dsquare_dx_expected}")
                self.assertEqual(dsquare_dx_enzyme, dsquare_dx_expected)

if __name__ == '__main__':
    unittest.main()
