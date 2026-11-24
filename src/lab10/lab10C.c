/*
Unix-Dude はルートキットを書くのが下手だ。

彼は get_r00t のコードを忘れたんだ q_q。
*/


#include <linux/module.h>    // すべてのカーネルモジュールで必要
#include <linux/kernel.h>    // KERN_INFO のために必要
#include <linux/init.h>      // __init と __exit マクロのために必要

#include <linux/fs.h>        // ファイルシステムの構造体のために必要
#include <linux/miscdevice.h>// デバイスドライバ関連のために必要 :
#include <linux/string.h>    // memcpy のため :)
#include <linux/random.h>    // get_random_bytes()

#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Patrick Biernat");
MODULE_DESCRIPTION("MBE Lab 10 C");

unsigned int secret;

typedef struct key_material {
    char key[1024];
    void (*algo)(char *);
    int auth;
}da_keyz;


struct key_material * sekret;

static ssize_t pwn_write(struct file* file, const char * buf, size_t count, loff_t *ppos)
{ // ここでパスワードを確認する

    printk(KERN_INFO "%s\n",buf);
    if( count == 0x31337) {
        if (sekret->auth) {
	     // ここで root に関する処理を行う。
	     printk(KERN_INFO "いい権限じゃん。\n");
        }
	return 0x31337;
    }
    
    if(buf[0] == '\x01') {
        printk("フラグは /root/flag にあります");
    }

    if ( ( *(unsigned int *)buf ^ 0xcafebabe) == 0) {
        printk(KERN_INFO "鍵の認証を行います...\n");
	sekret->algo(buf);
    }

    return count;
}


static const struct file_operations pwnops = {
    .owner  = THIS_MODULE,
    .write  = pwn_write,
};

static struct miscdevice pwn_dev = {
    MISC_DYNAMIC_MINOR, "pwn" , &pwnops
};

void algo_xor(char * buf) {
/*
    安全なワンタイムパッド認証関数。
*/
    int i;
    int sum;

    sum = 0;
    printk(KERN_INFO "algo_xor の中です!\n");
   
    for(i=0; i <= 1024; i++) {
        sekret->key[i] ^= buf[i];
    }
   
   for(i=0; i <= 1024; i++) {
        sum += sekret->key[i];
    }
    
    if(sum == 0) {
        sekret->auth = 1;
    }
    else {
        printk(KERN_INFO "認証に失敗しました!\n");
    	memset(sekret, 0 , sizeof(struct key_material));
    	get_random_bytes(&(sekret->key),1024);
    }
    return;
}

// おなじみの「main」関数のようなものだと考えてください
static int __init mod_init(void)
{
    int ret;
    // デバイスを /dev に登録する
    ret = misc_register(&pwn_dev);
    
    // 鍵用の領域を確保する 
    sekret = kmalloc(sizeof(struct key_material), GFP_KERNEL);
    get_random_bytes(&sekret->key, 1024);
   
    // アルゴリズムを妥当なデフォルト値に設定する
    sekret->algo = algo_xor;   

    printk(KERN_INFO "初期化が完了しました\n"); 
    printk(KERN_INFO "r00t を取れますか?\n");
    return 0;    
}

static void __exit mod_cleanup(void)
{
    printk(KERN_INFO "モジュールを後始末しています。\n");
    misc_deregister(&pwn_dev);
}


// ここで、モジュールをカーネルに「登録」している。
module_init(mod_init);
module_exit(mod_cleanup);


