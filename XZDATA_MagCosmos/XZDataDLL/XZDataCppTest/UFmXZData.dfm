object FmMain: TFmMain
  Left = 235
  Top = 221
  Width = 945
  Height = 545
  Caption = 'XZData Cpp Teat'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 14
    Top = 50
    Width = 293
    Height = 24
    AutoSize = False
    Caption = '交班 交易序號 99999'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 618
    Top = 436
    Width = 317
    Height = 25
    AutoSize = False
    Caption = 'V20210615'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label3: TLabel
    Left = 544
    Top = 90
    Width = 257
    Height = 24
    AutoSize = False
    Caption = '日結日歷日:YYYYMMDD'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label4: TLabel
    Left = 14
    Top = 90
    Width = 243
    Height = 24
    AutoSize = False
    Caption = '日結序號             起號:  '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label5: TLabel
    Left = 342
    Top = 90
    Width = 115
    Height = 24
    AutoSize = False
    Caption = ' ~      迄號:  '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label6: TLabel
    Left = 512
    Top = 52
    Width = 35
    Height = 24
    AutoSize = False
    Caption = ' ~     '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Memo1: TMemo
    Left = 4
    Top = 130
    Width = 921
    Height = 301
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    Lines.Strings = (
      'XZDATA_Mag20210601   雲端中獎發票新增獎項800元'
      ''
      'XZDATA_Mag20210423   目前針對1811 加值處理'
      ''
      'XZDATA_Mag20200831   目前針對1010 L0 洗選蛋不處理'
      ''
      'XZDATA_Mag20200409   目前針對1040  廢行動電源'
      ''
      
        'XZDATA_Mag20200304   目前只針對1040  該欄位=6排除交班短溢收計算' +
        '，需新增7'
      ''
      'XZDATA_Mag20191225  TM_PG_新增Fami Point支付帳表'
      ''
      ''
      '')
    ParentFont = False
    TabOrder = 0
  end
  object Button1: TButton
    Left = 8
    Top = 436
    Width = 119
    Height = 33
    Caption = 'CheckIn'
    TabOrder = 1
    OnClick = Button1Click
  end
  object EdiSalFile: TEdit
    Left = 10
    Top = 6
    Width = 665
    Height = 32
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 2
    Text = 'c:\FTLPOS\XZDATA\SAL\02.sal'
  end
  object Button2: TButton
    Left = 6
    Top = 472
    Width = 119
    Height = 33
    Caption = 'XReport'
    TabOrder = 3
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 128
    Top = 436
    Width = 119
    Height = 33
    Caption = 'XDT'
    TabOrder = 4
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 128
    Top = 472
    Width = 119
    Height = 33
    Caption = 'X_OK'
    TabOrder = 5
    OnClick = Button4Click
  end
  object Button5: TButton
    Left = 372
    Top = 436
    Width = 119
    Height = 33
    Caption = '日結'
    TabOrder = 6
    OnClick = Button5Click
  end
  object Button6: TButton
    Left = 372
    Top = 472
    Width = 119
    Height = 33
    Caption = 'Z_OK'
    TabOrder = 7
    OnClick = Button6Click
  end
  object Button7: TButton
    Left = 250
    Top = 436
    Width = 119
    Height = 33
    Caption = '重點商品'
    TabOrder = 8
    OnClick = Button7Click
  end
  object Edit1: TEdit
    Left = 326
    Top = 46
    Width = 173
    Height = 32
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 9
    Text = '00000000'
  end
  object Button8: TButton
    Left = 250
    Top = 472
    Width = 119
    Height = 33
    Caption = 'VXZDT:盤點讀帳'
    TabOrder = 10
    OnClick = Button8Click
  end
  object Button9: TButton
    Left = 492
    Top = 436
    Width = 119
    Height = 33
    Caption = '自動日結'
    TabOrder = 11
    OnClick = Button9Click
  end
  object Button10: TButton
    Left = 494
    Top = 472
    Width = 121
    Height = 33
    Caption = 'FileRead'
    TabOrder = 12
    OnClick = Button10Click
  end
  object EdAutoZDttm: TEdit
    Left = 806
    Top = 86
    Width = 123
    Height = 32
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 13
  end
  object Edi_ST: TEdit
    Left = 256
    Top = 86
    Width = 73
    Height = 32
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 14
    Text = '00000'
  end
  object Edi_ED: TEdit
    Left = 464
    Top = 86
    Width = 73
    Height = 32
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 15
    Text = '00000'
  end
  object Edit2: TEdit
    Left = 554
    Top = 46
    Width = 173
    Height = 32
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 16
    Text = '00000000'
  end
  object RadioButton1: TRadioButton
    Left = 692
    Top = 6
    Width = 145
    Height = 33
    Caption = '過濾重複資料'
    TabOrder = 17
  end
end
