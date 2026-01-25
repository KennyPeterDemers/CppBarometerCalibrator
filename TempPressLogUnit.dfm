object FormTempPressLog: TFormTempPressLog
  Left = 521
  Top = 297
  Caption = 'Temp-Pressure Log'
  ClientHeight = 340
  ClientWidth = 287
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  Position = poMainFormCenter
  TextHeight = 13
  object Label1: TLabel
    Left = 59
    Top = 312
    Width = 135
    Height = 13
    Caption = 'Log Record Delay (seconds)'
  end
  object MemoTempPressLog: TMemo
    Left = 0
    Top = 0
    Width = 287
    Height = 340
    Align = alClient
    Constraints.MinHeight = 281
    Constraints.MinWidth = 273
    ScrollBars = ssVertical
    TabOrder = 0
  end
  object CSpinEditTempPressLogDelay: TCSpinEdit
    Left = 11
    Top = 304
    Width = 41
    Height = 22
    MaxValue = 120
    MinValue = 1
    TabOrder = 1
    Value = 30
    OnChange = CSpinEditTempPressLogDelayChange
  end
  object MainMenu1: TMainMenu
    Left = 104
    object File1: TMenuItem
      Caption = '&File'
      object Save1: TMenuItem
        Caption = '&Save As...'
        OnClick = Save1Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = 'E&xit'
        OnClick = Exit1Click
      end
    end
    object View1: TMenuItem
      Caption = '&View'
      object Clear1: TMenuItem
        Caption = '&Clear'
        OnClick = Clear1Click
      end
      object Enabled1: TMenuItem
        Caption = 'Enabled'
        OnClick = Enabled1Click
      end
    end
  end
  object SaveDialog1: TSaveDialog
    Left = 184
  end
end
