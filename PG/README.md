# プログラミング言語

- c

# 開発環境

## 起動処理

kadai-\[version\]をコンパイルして実行してください

# TIPS

### 命名規則

グローバル変数はパスカルケース

```
Head,PowDiみたいなの
```

ローカル変数はキャメルケース

```
lastBoardNumsみたいなの
```

関数はスネークケース

```
find_next_boardsみたいなの
```

### graphql 定義を生成する

```
bin/rake graphql:schema:idl
```

### ユーザ(User)を生成する

```
bin/rake debug_user:create\[aaa@gmail.com\]

```

### ダミー決済履歴(Transaction)を生成する

```
shop_id, user_id, count
bin/rake debug_transaction:create\[2,2,10\]

```

### 全銀コードを更新する

https://github.com/zengin-code/zengin-rb
全銀コードはこちらの module で管理しています。下記コマンドで定期的にモジュールのアップデートを行い、データの最新化をしてください。

```
bundle update  zengin_code
```

### Postgres Database と postgis 設定

https://qiita.com/mist_dev/items/66c1684e07c2ef2c915e

```
$ brew install postgresql
$ brew install postgis
$ brew services start postgresql
```
