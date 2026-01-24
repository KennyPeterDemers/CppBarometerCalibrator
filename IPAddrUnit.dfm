object FormIPaddr: TFormIPaddr
  Left = 960
  Top = 321
  BorderStyle = bsDialog
  Caption = 'IP Addresses'
  ClientHeight = 193
  ClientWidth = 221
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label2: TLabel
    Left = 113
    Top = 32
    Width = 65
    Height = 13
    Caption = 'IP Addr (1-16)'
  end
  object Bevel2: TBevel
    Left = 9
    Top = 12
    Width = 200
    Height = 125
    Shape = bsFrame
  end
  object Label11: TLabel
    Left = 115
    Top = 96
    Width = 71
    Height = 13
    Caption = 'IP Addr (33-48)'
  end
  object Label7: TLabel
    Left = 110
    Top = 65
    Width = 71
    Height = 13
    Caption = 'IP Addr (17-32)'
  end
  object EditAddr1: TEdit
    Left = 22
    Top = 24
    Width = 84
    Height = 21
    ReadOnly = True
    TabOrder = 0
    Text = '192.168.1.150'
    OnKeyPress = EditAddr1KeyPress
  end
  object EditAddr2: TEdit
    Left = 22
    Top = 56
    Width = 84
    Height = 21
    ReadOnly = True
    TabOrder = 1
    Text = '192.168.1.151'
    OnKeyPress = EditAddr2KeyPress
  end
  object EditAddr3: TEdit
    Left = 22
    Top = 88
    Width = 84
    Height = 21
    ReadOnly = True
    TabOrder = 2
    Text = '192.168.1.152'
    OnKeyPress = EditAddr3KeyPress
  end
  object Button1: TButton
    Left = 72
    Top = 152
    Width = 75
    Height = 25
    Caption = '&Ok'
    TabOrder = 3
    OnClick = Button1Click
  end
end
