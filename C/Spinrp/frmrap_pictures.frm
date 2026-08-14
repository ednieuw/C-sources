VERSION 5.00
Begin VB.Form frmrap_pictures 
   Caption         =   "RAP_Pictures"
   ClientHeight    =   7356
   ClientLeft      =   1104
   ClientTop       =   288
   ClientWidth     =   11700
   LinkMode        =   1  'Source
   LinkTopic       =   "Form3"
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   7356
   ScaleWidth      =   11700
   Begin VB.CommandButton cmdClose 
      Caption         =   "&Close"
      Height          =   300
      Left            =   960
      TabIndex        =   22
      Top             =   3720
      Width           =   975
   End
   Begin VB.Data Data1 
      Align           =   2  'Align Bottom
      Connect         =   "Access"
      DatabaseName    =   "D:\Eri\Spinnen.mdb"
      DefaultCursorType=   0  'DefaultCursor
      DefaultType     =   2  'UseODBC
      Exclusive       =   0   'False
      Height          =   324
      Left            =   0
      Options         =   0
      ReadOnly        =   0   'False
      RecordsetType   =   1  'Dynaset
      RecordSource    =   "RAP_Pictures"
      Top             =   7032
      Width           =   11700
   End
   Begin VB.TextBox txtFields 
      DataField       =   "SRT"
      DataSource      =   "Data1"
      Height          =   285
      Index           =   9
      Left            =   720
      MaxLength       =   50
      TabIndex        =   21
      Top             =   2640
      Width           =   1932
   End
   Begin VB.TextBox txtFields 
      DataField       =   "NL"
      DataSource      =   "Data1"
      Height          =   285
      Index           =   8
      Left            =   720
      MaxLength       =   50
      TabIndex        =   19
      Top             =   3360
      Width           =   1932
   End
   Begin VB.TextBox txtFields 
      DataField       =   "Geslacht"
      DataSource      =   "Data1"
      Height          =   285
      Index           =   7
      Left            =   1200
      TabIndex        =   17
      Top             =   2964
      Width           =   852
   End
   Begin VB.TextBox txtFields 
      DataField       =   "GEN"
      DataSource      =   "Data1"
      Height          =   285
      Index           =   6
      Left            =   720
      MaxLength       =   50
      TabIndex        =   15
      Top             =   2280
      Width           =   1932
   End
   Begin VB.TextBox txtFields 
      DataField       =   "FAM"
      DataSource      =   "Data1"
      Height          =   285
      Index           =   5
      Left            =   720
      MaxLength       =   50
      TabIndex        =   13
      Top             =   1960
      Width           =   1932
   End
   Begin VB.CheckBox chkFields 
      DataField       =   "HasPicture"
      DataSource      =   "Data1"
      Height          =   285
      Left            =   1200
      TabIndex        =   11
      Top             =   1640
      Width           =   972
   End
   Begin VB.TextBox txtFields 
      DataField       =   "CDVnr"
      DataSource      =   "Data1"
      Height          =   285
      Index           =   4
      Left            =   1200
      TabIndex        =   9
      Top             =   1320
      Width           =   492
   End
   Begin VB.TextBox txtFields 
      DataField       =   "CDId"
      DataSource      =   "Data1"
      Height          =   285
      Index           =   3
      Left            =   1200
      TabIndex        =   7
      Top             =   1000
      Width           =   492
   End
   Begin VB.TextBox txtFields 
      DataField       =   "Nummer"
      DataSource      =   "Data1"
      Height          =   285
      Index           =   2
      Left            =   1200
      TabIndex        =   5
      Top             =   680
      Width           =   732
   End
   Begin VB.TextBox txtFields 
      DataField       =   "MediumId"
      DataSource      =   "Data1"
      Height          =   285
      Index           =   1
      Left            =   1200
      TabIndex        =   3
      Top             =   360
      Width           =   972
   End
   Begin VB.TextBox txtFields 
      DataField       =   "ERId"
      DataSource      =   "Data1"
      Height          =   285
      Index           =   0
      Left            =   1200
      TabIndex        =   1
      Top             =   40
      Width           =   972
   End
   Begin VB.OLE OLE1 
      Class           =   "PSP5.Image"
      DataSource      =   "Data1"
      Height          =   3792
      Left            =   2760
      OleObjectBlob   =   "frmrap_pictures.frx":0000
      SizeMode        =   3  'Zoom
      TabIndex        =   23
      Top             =   120
      Width           =   5736
   End
   Begin VB.Label lblLabels 
      Caption         =   "SRT:"
      Height          =   252
      Index           =   10
      Left            =   120
      TabIndex        =   20
      Top             =   2640
      Width           =   492
   End
   Begin VB.Label lblLabels 
      Caption         =   "NL:"
      Height          =   252
      Index           =   9
      Left            =   120
      TabIndex        =   18
      Top             =   3420
      Width           =   492
   End
   Begin VB.Label lblLabels 
      Caption         =   "Geslacht:"
      Height          =   252
      Index           =   8
      Left            =   120
      TabIndex        =   16
      Top             =   2976
      Width           =   1812
   End
   Begin VB.Label lblLabels 
      Caption         =   "GEN:"
      Height          =   255
      Index           =   7
      Left            =   120
      TabIndex        =   14
      Top             =   2300
      Width           =   1815
   End
   Begin VB.Label lblLabels 
      Caption         =   "FAM:"
      Height          =   255
      Index           =   6
      Left            =   120
      TabIndex        =   12
      Top             =   1980
      Width           =   1815
   End
   Begin VB.Label lblLabels 
      Caption         =   "HasPicture:"
      Height          =   255
      Index           =   5
      Left            =   120
      TabIndex        =   10
      Top             =   1660
      Width           =   1815
   End
   Begin VB.Label lblLabels 
      Caption         =   "CDVnr:"
      Height          =   255
      Index           =   4
      Left            =   120
      TabIndex        =   8
      Top             =   1340
      Width           =   1815
   End
   Begin VB.Label lblLabels 
      Caption         =   "CDId:"
      Height          =   255
      Index           =   3
      Left            =   120
      TabIndex        =   6
      Top             =   1020
      Width           =   1815
   End
   Begin VB.Label lblLabels 
      Caption         =   "Nummer:"
      Height          =   255
      Index           =   2
      Left            =   120
      TabIndex        =   4
      Top             =   700
      Width           =   1815
   End
   Begin VB.Label lblLabels 
      Caption         =   "MediumId:"
      Height          =   255
      Index           =   1
      Left            =   120
      TabIndex        =   2
      Top             =   380
      Width           =   1815
   End
   Begin VB.Label lblLabels 
      Caption         =   "ERId:"
      Height          =   255
      Index           =   0
      Left            =   120
      TabIndex        =   0
      Top             =   60
      Width           =   1815
   End
End
Attribute VB_Name = "frmrap_pictures"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub cmdAdd_Click()
  Data1.Recordset.AddNew
End Sub

Private Sub cmdDelete_Click()
  'this may produce an error if you delete the last
  'record or the only record in the recordset
  Data1.Recordset.Delete
  Data1.Recordset.MoveNext
End Sub

Private Sub cmdRefresh_Click()
  'this is really only needed for multi user apps
  Data1.Refresh
End Sub

Private Sub cmdUpdate_Click()
  Data1.UpdateRecord
  Data1.Recordset.Bookmark = Data1.Recordset.LastModified
End Sub

Private Sub cmdClose_Click()
  Unload Me
End Sub

Private Sub Data1_Error(DataErr As Integer, Response As Integer)
  'This is where you would put error handling code
  'If you want to ignore errors, comment out the next line
  'If you want to trap them, add code here to handle them
  MsgBox "Data error event hit err:" & Error$(DataErr)
  Response = 0  'throw away the error
End Sub

Private Sub Data1_Reposition()
  Screen.MousePointer = vbDefault
  On Error Resume Next
  'This will display the current record position
  'for dynasets and snapshots
  Data1.Caption = "Record: " & (Data1.Recordset.AbsolutePosition + 1)
  'for the table object you must set the index property when
  'the recordset gets created and use the following line
  'Data1.Caption = "Record: " & (Data1.Recordset.RecordCount * (Data1.Recordset.PercentPosition * 0.01)) + 1
End Sub

Private Sub Data1_Validate(Action As Integer, Save As Integer)
  'This is where you put validation code
  'This event gets called when the following actions occur
    Text = "D:\ERI\PICED\" + txtFields(1) + Format(txtFields(2), "0000") + ".jpg"
    If Len(Dir(Text)) = 0 Then
        Text = "D:\ERI\cd" & Format(txtFields(3), "000") & "\img" & Format(txtFields(4), "0000") & ".jpg"
    End If
    OLE1.CreateLink (Text)
  Select Case Action
    Case vbDataActionMoveFirst
    Case vbDataActionMovePrevious
    Case vbDataActionMoveNext
    Case vbDataActionMoveLast
    Case vbDataActionAddNew
    Case vbDataActionUpdate
    Case vbDataActionDelete
    Case vbDataActionFind
    Case vbDataActionBookmark
    Case vbDataActionClose

  End Select
  Screen.MousePointer = vbHourglass
End Sub

Private Sub OLE2_Updated(Code As Integer)

End Sub

Private Sub File1_Click()

End Sub
