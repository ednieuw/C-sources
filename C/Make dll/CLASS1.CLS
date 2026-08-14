VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "Class1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Function Test(Rows() As Double, RowsN As Integer, R1 As Double, R2 As Double, R3 As Double) As Integer
    Dim n As Integer
    
    R1 = 0
    R2 = 0
    R3 = 0
    
    For n = 0 To RowsN
        R1 = R1 + Rows(n)
        R2 = R2 + 1
    Next n
    
    R3 = R2 / R1
    Test = 1
End Function
