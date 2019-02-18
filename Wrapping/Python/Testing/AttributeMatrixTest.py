

from dream3d import simpl

import dream3d.utils.simpl_common as sc
import dream3d.utils.simpl_test_dirs as sd


def AttributeMatrixTest():
    # Create an AttributeMatrix
    amType = simpl.AttributeMatrix.Type.Cell
    tupleDims = simpl.VectorSizeT([5, 4, 3])
    amName = "CellAttributeMatrix"
    am = sc.CreateAttributeMatrix(tupleDims, amName, amType)

    print("Name: %s" % am.Name)

    arrayExist = am.doesAttributeArrayExist("Test Data Array")
    assert arrayExist is False
    print("arrayExist: %s" % arrayExist)


"""
Main entry point for python script
"""
if __name__ == "__main__":
    AttributeMatrixTest()
    print("AttributeMatrixTest Test Complete")
