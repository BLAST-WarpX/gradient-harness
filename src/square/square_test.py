# To run tests:
# make pybind
# python3 -m unittest -v square_test

import unittest
import square

def dsquare_analytic(x):
    return 2*x

class SquareTest(unittest.TestCase):
    def test_dsquare(self):
        """
        Test d(square)/dx for multiple values
        """
        test_vals =  [-1.0, 0.0, 1.5, 3.2, 27.876]
        
        for k, test_val in enumerate(test_vals):
            with self.subTest(f"x ={test_val}", i=k):
                dsquare_enzyme = square.dsquare(test_val)
                dsquare_expected = dsquare_analytic(test_val)
            
                print(f"(i={k}) x = {test_val}; d(square)/dx Enzyme = {dsquare_enzyme}; d(square)/dx Expected = {dsquare_expected}")
                self.assertEqual(dsquare_enzyme, dsquare_expected)

if __name__ == '__main__':
    unittest.main()
