VERSION 5.00
Object = "{6B7E6392-850A-101B-AFC0-4210102A8DA7}#1.2#0"; "COMCTL32.OCX"
Begin VB.Form Form1 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Logit for Excel"
   ClientHeight    =   1260
   ClientLeft      =   1488
   ClientTop       =   1440
   ClientWidth     =   3960
   Icon            =   "Form1.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1260
   ScaleWidth      =   3960
   Begin ComctlLib.ProgressBar ProgressBar2 
      Height          =   252
      Left            =   291
      TabIndex        =   2
      Top             =   36
      Width           =   3372
      _ExtentX        =   5948
      _ExtentY        =   445
      _Version        =   327682
      Appearance      =   0
   End
   Begin ComctlLib.ProgressBar ProgressBar1 
      Height          =   252
      Left            =   297
      TabIndex        =   1
      Top             =   972
      Width           =   3372
      _ExtentX        =   5948
      _ExtentY        =   445
      _Version        =   327682
      Appearance      =   0
   End
   Begin VB.CommandButton Loaddata 
      Caption         =   "Start"
      Height          =   456
      Left            =   1371
      TabIndex        =   0
      Top             =   396
      Width           =   1212
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
' Declare necessary API routines:
Private Declare Function FindWindow Lib "user32" Alias _
"FindWindowA" (ByVal lpClassName As String, _
                    ByVal lpWindowName As Long) As Long
Private Declare Function SendMessage Lib "user32" Alias _
"SendMessageA" (ByVal hWnd As Long, _
                ByVal wMsg As Long, _
                ByVal wParam As Long, _
                ByVal lParam As Long) As Long
Option Base 1
Public MAX_YK_PNT, rows As Integer
Public slope As Double, intercept As Double, corr As Double
Public Bmax As Double, Blank As Double, ymax As Double, ymin As Double

Dim Dataarray(150, 2) As Double                         'Gegevens regressie data
Dim MyXL As Object                                      'Declare variable to hold the reference.
                                       'in Project --> reference MS excel8.0 selecteren
                                                        'Use MyXL to access Microsoft Excel's other objects.

Private Sub Form_Load()
GetExcel
End Sub

Private Sub Form_Unload(Cancel As Integer)
'MyXL.Quit  ' When you finish, use the Quit method to close
Set MyXL = Nothing ' the application, then release the reference.
End Sub

Private Sub Loaddata_Click()
berekend = False                                              'Detect is Excel is running, if not start it with test.xls
MyXL.Application.ScreenUpdating = False
Logitregressie                                          'Bereken regressieparameters
Results_in_Excel
MyXL.Application.ScreenUpdating = True

End Sub


Sub GetExcel()
    ' Dim MyXL As Object                                ' Variable to hold reference to Microsoft Excel.
    Dim ExcelWasNotRunning As Boolean                   ' Flag for final release.

                                                        ' Test to see if there is a copy of Microsoft Excel already running.
    On Error Resume Next                                ' Defer error trapping.
                                                        ' Getobject function called without the first argument returns a
                                                        ' reference to an instance of the application. If the application isn't
                                                        ' running, an error occurs.
    Set MyXL = GetObject(, "Excel.Application")
    If Err.Number <> 0 Then ExcelWasNotRunning = True
    Err.Clear                                           ' Clear Err object in case error occurred.
                                                        ' Check for Microsoft Excel. If Microsoft Excel is running,
                                                        ' enter it into the Running Object table.
    DetectExcel
' Show Microsoft Excel through its Application property. Then
' show the actual window containing the file using the Windows
' collection of the MyXL object reference.
    If ExcelWasNotRunning = True Then
'Set the object variable to reference the file you want to see.
    Set MyXL = GetObject("Default.xls")
    MyXL.Application.Visible = True
    MyXL.Parent.Windows(1).Visible = True
    End If
    
    ' Do manipulations of your
    ' file here.

' ...
' If this copy of Microsoft Excel was not running when you
' started, close it using the Application property's Quit method.
' Note that when you try to quit Microsoft Excel, the
' title bar blinks and a message is displayed asking if you
' want to save any loaded files.
'
'        MyXL.Application.Quit
'    End If

'    Set MyXL = Nothing  ' Release reference to the
    ' application and spreadsheet.
End Sub




Sub DetectExcel()

' Procedure dectects a running Excel and registers it.
    Const WM_USER = 1024
    Dim hWnd As Long
' If Excel is running this API call returns its handle.
    hWnd = FindWindow("XLMAIN", 0)
    If hWnd = 0 Then    ' 0 means Excel not running.
        Exit Sub
    Else
    ' Excel is running so use the SendMessage API
    ' function to enter it in the Running Object Table.
        SendMessage hWnd, WM_USER + 18, 0, 0
    End If
End Sub

  Sub Logitregressie()
  
Dim k As Integer
Dim lus As Integer, iter As Integer
Dim Vx As Double, vy As Double
Dim somxx As Double, somxy As Double, somyy As Double
Dim sx As Double, sy As Double, sxy As Double
Dim corroud As Double, xmax As Double, yc As Integer
Dim Vxx(151) As Double
Dim factor As Double, maalfactor As Double
Dim DataOk As Integer

factor = 1.1
maalfactor = 1.01
corroud = 0
corr = 0

DataOk = True               'Als er en punt verwijderd wordt data opnieuw inlezen

'While berekend = False
Laad_opnieuw:
    lus = 0
    Get_data_from_Excel          ' Haal de gegevens uit Excel
    lus = lus + 1
'    maxregel = 0
'    lastregel = 1                               'nodig bij uitrekenen om tellers te resetten
    xmax = ymax = Bmax = yc = 0#
    ymin = 10000000000#
    corr = 1#
        
                        ' if rows_temp<3 and aantal<3 then fill_Dataarray(hwnd,i,j);
    For k = 1 To rows       ' Bereken extremen
        If Dataarray(k, 1) > xmax Then xmax = Dataarray(k, 1)
        If Dataarray(k, 2) > ymax Then ymax = Dataarray(k, 2)
        If Dataarray(k, 2) < ymin Then ymin = Dataarray(k, 2)
    Next k
    Bmax = ymax * 1.001
    somx = 0:    somy = 0:    somxx = 0:    somyy = 0:    somxy = 0:    aantal = 0
    For k = 1 To rows    'Bepaal of stijgende of dalende regressielijn
         vy = Dataarray(k, 2)
         Vx = Dataarray(k, 1)
         aantal = aantal + 1
         somx = somx + Vx
         somy = somy + vy
         somxy = somxy + Vx * vy
         somxx = somxx + Vx * Vx
    Next k

    sxy = (somxy - (somx * somy / aantal))
    sx = (somxx - (somx * somx / aantal))
    slope = sxy / sx
    If slope < 0 Then
        yc = -1
        Else: yc = 1
    End If
 
    For k = 1 To rows
        If yc > 0 And Dataarray(k, 2) <= Blank Then
            Verwijder_punt (k) 'gooi punt eruit
            DataOk = False
            End If
         If yc < 0 And Dataarray(k, 2) >= Blank Then
            Verwijder_punt (k) 'gooi punt eruit
            DataOk = False
            End If
    Next k
    If DataOk = False Then
        DataOk = True
        GoTo Laad_opnieuw
        End If
                'start logit regressie
    
 somx = 0:    somy = 0:    somxx = 0:    somyy = 0:    somxy = 0:    aantal = 0
        
    For k = 1 To rows
        Vxx(k) = Log(Dataarray(k, 1))
        vy = Dataarray(k, 2)
        somx = somx + Vxx(k)
        somxx = somxx + Vxx(k) * Vxx(k)
        aantal = aantal + 1
        Next k
 
    If yc < 0# Then
        D = ymin / factor
        Else: D = ymax * factor
        End If
    iter = 0
 
    While lus >= 0                       'Ga itereren
        iter = iter + 1
        somy = 0
        somyy = 0
        somxy = 0
        For k = 1 To rows
            vy = logit(Dataarray(k, 2))
            somy = somy + vy
            somyy = somyy + vy * vy
            somxy = somxy + Vxx(k) * vy
        Next k
        sxy = (somxy - (somx * somy / aantal))
        sx = (somxx - (somx * somx / aantal))
        sy = (somyy - (somy * somy / aantal))
        corr = (sxy / (Sqr(sx * sy)))
        slope = sxy / sx
        intercept = (somy / aantal) - (somx / aantal) * slope
        If corr < 0 Then
            corr = 0 - corr       'maak R positief
            End If
        factor = factor * maalfactor
        If corr <= corroud Then
            maalfactor = 1
            lus = lus + 1                ' verhoog de loop teler*/
            End If
        If lus = 2 Then
            factor = (((factor - 1) / 10) + 1) ' Ga nog max 10 keer terug */
            yc = -yc                   ' In veranderde richting */
            End If
        If lus = 3 Then
            factor = (((factor - 1) / 5) + 1) ' Ga nog max 5 keer terug */
            yc = -yc                   ' In veranderde richting */
            End If
        If lus = 4 Then
                                       ' Ga nog 1 keer terug */
            yc = -yc                   ' In veranderde richting */
            End If
 '  End If
        If lus <= 4 Then
            corroud = corr
            Else: berekend = True
            GoTo Einde
            End If                                  ' Als loop>3 verlaat iteratie*/
        If yc > 0 Then
            Bmax = Bmax * factor                    ' D verlagen of verhogen ? */
            Else: Bmax = Bmax / factor
            End If
        If Bmax > Blank And Bmax <= ymax Then
            D = ymax * 1.01                      ' D niet < ymax */
            End If
        If iter > 1000 Then
            GoTo Einde
            End If
  
    Wend   ' while lus > 0
' Wend                                             ' einde if berekend */

Einde:
 corr = sxy / Sqr(sx * sy)
 slope = sxy / sx
 intercept = (somy / aantal) - (somx / aantal) * slope
 berekend = True
End Sub
  
Sub Verwijder_punt(n)
With MyXL.Application.ActiveSheet
    .Cells(n, 3).Value = .Cells(n, 2).Value             'Verplaats getal
    .Cells(n, 2).Value = Null
    .Range("b" + CStr(n)).Font.Bold = False
    .Range("b" + CStr(n)).Font.Italic = False
End With
End Sub

Sub Get_data_from_Excel()
With MyXL.Application.ActiveSheet
Cls
rows = 1
Blank = 0
ProgressBar2.Min = 1
ProgressBar2.Max = 50
For n = 1 To 50
ProgressBar2.Value = n
DoEvents
    If IsNumeric(.Cells(n, 1).Value) = False Then GoTo endnext
    If IsEmpty(.Cells(n, 1).Value) Then GoTo endnext    'lege cell in eerste kolom
    If .Cells(n, 1).Value < 0 Then GoTo endnext
    If IsEmpty(.Cells(n, 2).Value) Then GoTo endnext    'lege cel in tweede kolom
    If IsNumeric(.Cells(n, 1).Value) Then                'staat er een getal in kolom een?
        If .Range("b" + CStr(n)).Font.Italic = True Then    'staat er een verwijderd getal (italic = dataset niet gebruiken)
            Verwijder_punt (n)
            GoTo endnext
            End If
        If .Cells(n, 1).Value = 0 Then
            Blank = .Cells(n, 2).Value
            Else:   Dataarray(rows, 1) = .Cells(n, 1).Value
                    Dataarray(rows, 2) = .Cells(n, 2).Value
  '                  Print Dataarray(rows, 1), Dataarray(rows, 2)
                    rows = rows + 1
                    End If
    'Debug.Print Dataarray(n, m);
        End If
endnext:
Next n
End With
rows = rows - 1
End Sub

Function logit(meet) As Double
 If (meet - Blank) / (Bmax - meet) <= 0 Then
 logit = 99999
 Else
 logit = Log(((meet - Blank) / (Bmax - meet)))
 End If
End Function
Sub Results_in_Excel()
Dim y As Double
Dim x As Double
Dim stap As Double
Dim line As Integer
Dim ymaxtemp As Double, ymintemp As Double
Dim colx As Integer, coly As Integer
With MyXL.Application.ActiveSheet

 coly = 1: colx = 6
.Cells(coly, colx - 1).Value = "Logit regression VB1.0 Jan 1998"
.Cells(coly + 1, colx).Value = "Correlation"
.Cells(coly + 2, colx).Value = "Slope"
.Cells(coly + 3, colx).Value = "Intercept"
.Cells(coly + 4, colx).Value = "Blank"
.Cells(coly + 5, colx).Value = "Bmax"
.Cells(coly + 1, colx + 1).Value = corr
.Cells(coly + 2, colx + 1).Value = slope
.Cells(coly + 3, colx + 1).Value = intercept
.Cells(coly + 4, colx + 1).Value = Blank
.Cells(coly + 5, colx + 1).Value = Bmax

line = 0
ymintemp = ymin
ymaxtemp = Bmax
If Bmax > 1.2 * ymax Then ymaxtemp = 1.2 * ymax
If ymax < Blank Then ymaxtemp = Blank * 0.99999
If ymin > Blank Then ymintemp = Blank + 0.000000001
   .Range("L1:M30").ClearContents
For y = ymintemp To ymaxtemp * 1.001 Step (ymaxtemp - ymintemp) / 20#
    'logit (y)
    x = Exp((logit(y) - intercept) / slope)
    line = line + 1
    .Cells(line, 22).Value = x
    .Cells(line, 23).Value = y
    Next y
End With
Put_results_in_Excel
End Sub
Sub Put_results_in_Excel()
With MyXL.Application.ActiveSheet
Dim val As Double
ProgressBar1.Min = 1
ProgressBar1.Max = 200

For n = 1 To 200
ProgressBar1.Value = n
    If IsNumeric(.Cells(n, 2).Value) = False Then GoTo endnext
    If IsEmpty(.Cells(n, 2).Value) Then GoTo endnext      'lege cell in tweede datakolom
    If IsNumeric(.Cells(n, 2).Value) Then
    val = .Cells(n, 2).Value                                'staat er een getal in kolom twee
        If val <= Blank Then
        .Cells(n, 4).Value = "<< >>"
        GoTo endnext
        End If
        If val >= Bmax Then
        .Cells(n, 4).Value = "<< >>"
        GoTo endnext
        End If
        .Cells(n, 4).Value = Exp((logit(val) - intercept) / slope)
    End If
endnext:
Next n
End With
Beep
End Sub
