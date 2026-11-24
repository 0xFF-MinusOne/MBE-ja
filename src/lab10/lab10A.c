/*
iptables はダメだ。 
*/


#include <linux/module.h>    // すべてのカーネルモジュールで必要
#include <linux/kernel.h>    // KERN_INFO のために必要
#include <linux/init.h>      // __init と __exit マクロのために必要

#include <linux/fs.h>        // ファイルシステムの構造体のために必要
#include <linux/miscdevice.h>// デバイスドライバ関連のために必要 :
#include <linux/string.h>    // memcpy のため :)
#include <linux/random.h>    // get_random_bytes()

// ネットワーク関連のインクルード
#include <net/protocol.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/skbuff.h>
#include <linux/ip.h>

#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Patrick Biernat");
MODULE_DESCRIPTION("MBE Lab 10A");


typedef struct disallow_filter {
    unsigned int pr_num;
    unsigned int port;
    unsigned int id;
    struct disallow_filter * next;
    struct disallow_filter * prev;
}d;

typedef struct callback_filter {
    unsigned int pr_num;
    unsigned int id;

    void (*callback)(struct callback_filter *);
    struct callback_filter * next;
    struct callback_filter * prev;
}c;

struct disallow_filter * dfilter_head;
struct callback_filter * cfilter_head;
struct nf_hook_ops nfho;

unsigned int pkt_hook(unsigned int hooknum, struct sk_buff *skb, const struct 
             net_device * in, const struct net_device * out, 
	                  int (*okfn)(struct sk_buff *)) {
/*
    この関数は、システムに入ってくるパケットをフィルタリングします。
    コールバックフィルタは、拒否フィルタ (disallow filter) より優先されます。
*/
    
    struct disallow_filter * walkera;
    struct callback_filter * walkerb;
    struct iphdr * ip_header;
    
    printk(KERN_INFO "[uTables] パケットを受信しました!\n");

    ip_header = (struct iphdr *)skb_network_header(skb);

    walkera = dfilter_head;
    walkerb = cfilter_head;
    
    while(walkerb != 0)  {
        if(walkerb->pr_num == ip_header->protocol) {
            break;
        }
        walkerb = walkerb->next;
    }
    if (walkerb) {
        walkerb->callback(walkerb);
        return NF_ACCEPT;
    }

    while(walkera != 0) {
        if (walkera->pr_num == ip_header->protocol) {
            break;
        }
        walkera = walkera->next;
    }
    if (walkera) {
        printk(KERN_INFO "[uTables] 拒否対象のパケットを検出しました!\n");
        return NF_DROP;
    }


    return NF_ACCEPT;
}

void emulate_packet(int pnum) {
    struct disallow_filter * walkera;
    struct callback_filter * walkerb;
    printk(KERN_INFO "[uTables] プロトコルをエミュレートします: %d\n", pnum);
    

    walkera = dfilter_head;
    walkerb = cfilter_head;
    
    while(walkerb != 0)  {
        if(walkerb->pr_num == pnum) {
            break;
        }
        walkerb = walkerb->next;
    }
    if (walkerb) {
        printk(KERN_INFO "コールバックフィルタが見つかりました!\n");
        walkerb->callback(walkerb);
        return;
    }

    while(walkera != 0) {
        if (walkera->pr_num == pnum) {
            break;
        }
        walkera = walkera->next;
    }
    if (walkera) {
        printk(KERN_INFO "[uTables] 拒否対象のパケットを検出しました!\n");
        return;
    }
    return;
}

void default_callback(struct sk_buff  * skb) {
    printk(KERN_INFO "[uTables] デフォルトのコールバックが呼ばれました!\n");
    return;
}

static ssize_t pwn_read(struct file* file, char * buf, size_t count, loff_t *ppos)
{
    return count;
}

int add_cfilter(struct callback_filter * filt) {
/*
    uTables にコールバックフィルタを追加します。
*/
    struct callback_filter * nfilter, *walker;
    printk("[uTables] コールバックフィルタを追加します\n");
    
    nfilter = kmalloc(sizeof(struct callback_filter), GFP_KERNEL);
    nfilter->pr_num = filt->pr_num;
    nfilter->id = filt->id;
    nfilter->callback = default_callback;
    nfilter->next = 0;

    walker = cfilter_head;
    while(walker->next != 0) {
        walker = walker->next;
    }
    walker->next = nfilter;
    nfilter->prev = walker;
    
    printk(KERN_INFO "新しいフィルタのアドレス: %p\n", nfilter);
    printk(KERN_INFO "新しいフィルタのプロトコル: %u\n", nfilter->pr_num);
    printk(KERN_INFO "新しいフィルタの ID: %u\n", nfilter->id);
    printk(KERN_INFO "新しいフィルタのコールバック @: %p\n", nfilter->callback);
    printk(KERN_INFO "新しいフィルタの NEXT: %p\n", nfilter->next);
    printk(KERN_INFO "新しいフィルタの PREV: %p\n", nfilter->prev);

    return 0;

}

int add_dfilter(struct disallow_filter * filt) {
/*
    uTables に拒否フィルタを追加します。
*/

    struct disallow_filter * nfilter, *walker;
    
    printk("PR NUM: %d\n", filt->pr_num);
    nfilter = kmalloc(sizeof(struct disallow_filter), GFP_KERNEL);
    nfilter->pr_num = filt->pr_num;    
    nfilter->port = filt->port;
    nfilter->id = filt->id;
    nfilter->next = 0;

    walker = dfilter_head;
    while(walker->next != 0) {
        walker = walker->next;
    }
    walker->next = nfilter;
    nfilter->prev = walker;
    
    printk(KERN_INFO "新しいフィルタのアドレス: %p", nfilter);
    printk(KERN_INFO "新しいフィルタのプロトコル: %u\n", nfilter->pr_num);
    printk(KERN_INFO "新しいフィルタのポート: %u\n", nfilter->port);
    printk(KERN_INFO "新しいフィルタの ID: %u\n", nfilter->id);

    return 0;
}

int remove_defilter(int pr_num) {
/*
     指定されたプロトコル番号のフィルタを削除します。 
*/
     struct disallow_filter * walker;

     walker = dfilter_head;
     while(walker->pr_num != pr_num && walker != NULL){
         walker = walker->next;
     }
     if(walker) {
         printk(KERN_INFO "%u のフィルタを %08x から削除します\n", walker->pr_num, walker);
         walker->prev->next = walker->next;
	 if(walker->next) {
	     walker->next->prev = walker->prev;
         }
         kfree(walker);
         return 0;
     }
    return -1;
}

int modify_callback(void * data) {
/*
    デフォルトのコールバックとは別に、
    追加のカーネルモジュールをロードして、その関数を
    ここでフィルタリングフックからのコールバックルーチンとして使うこともできます。
*/
    struct callback_filter * walker;
    unsigned int addr;
    unsigned int cid;

    memcpy(&cid, data, 4);
    memcpy(&addr, data + 4, 4);
    printk(KERN_INFO "Addr: %u\n", addr);
    printk(KERN_INFO "ID: %u\n", cid);
    
    if (addr < 0xc0000000) {
        printk(KERN_INFO "[ERROR] ユーザー空間のアドレスを検出しました!\n");
        return -1;
    }

    walker = cfilter_head;
    while (walker != 0) {
        if (walker->id == cid) {
            break;
        }
        walker = walker->next;
    }
    if (walker) {
        walker->callback = addr;
        printk(KERN_INFO "フィルタ ID %u のコールバックを更新しました: %u\n",walker->id, walker->callback);
    }
    else {
        printk(KERN_INFO "ID %u のフィルタが見つかりませんでした\n",walker->id);
        return -1;
    }
    return 0;
}

static ssize_t pwn_write(struct file* file, const char * buf, size_t count, loff_t *ppos)
{ // uTables はこのようにして設定します
   
    if (buf[0] == '\x01') { // これで、この相手と話していることが分かります :)
        printk(KERN_INFO "フラグは /root/flag にあります\n");
    }

    if (buf[0] == '\x02') { // 拒否フィルタを追加する
        printk(KERN_INFO "[uTables] 新しい拒否フィルタを追加します\n");
        printk(KERN_INFO "PR: %u", buf[1]);
        add_dfilter( (struct disallow_filter *) &(buf[1]));
    }

    if (buf[0] == '\x03') { // コールバックフィルタを追加する
        printk(KERN_INFO "[uTables] 新しいコールバックフィルタを追加します\n");
        add_cfilter( (struct callback_filter *) &(buf[1]));
    }

    if (buf[0] == '\x04') { // コールバック関数を変更する
        printk(KERN_INFO "[uTables] コールバックを変更します\n");
        modify_callback((void *) &(buf[4]));
    }

    if (buf[0] == '\x05') { // カーネルにパケットが到着したことをエミュレートする。
        printk(KERN_INFO "[uTables] パケットの到着をエミュレートします\n");
        emulate_packet( (int) (buf[1]));

    }

    return count;
}


static const struct file_operations pwnops = {
    .owner  = THIS_MODULE,
    .read   = pwn_read,
    .write  = pwn_write,
};

static struct miscdevice pwn_dev = {
    MISC_DYNAMIC_MINOR, "pwn" , &pwnops
};

static int __init mod_init(void)
{
/*
    データ構造を初期化し、uTables を設定するために使うデバイスを 
    登録します。
    また、カーネルに入ってくるパケットを見られるように、
    ネットワークのフックも設定します。
*/

    int ret;
    // デバイスを登録し、連結リストの先頭を作る 
    ret = misc_register(&pwn_dev); 
    dfilter_head = kmalloc(sizeof(struct disallow_filter), GFP_KERNEL);
    cfilter_head = kmalloc(sizeof(struct callback_filter), GFP_KERNEL);
   
    // 連結リストの先頭を初期化する
    dfilter_head->pr_num = -1;
    dfilter_head->port = -1;
    dfilter_head->next = 0;
    dfilter_head->prev = 0;

    cfilter_head->pr_num = -1;
    cfilter_head->id = -1;
    cfilter_head->callback = default_callback;
    cfilter_head->next = 0;
    cfilter_head->prev = 0;

    // 基本的な netfilter フックを設定する
    nfho.hook = pkt_hook;
    nfho.hooknum = 0;
    nfho.pf = PF_INET;
    nfho.priority = NF_IP_PRI_FIRST;

    // フックを登録する 
    nf_register_hook(&nfho);

    printk(KERN_INFO "初期化が完了しました\n"); 
    printk(KERN_INFO "r00t を取れますか?\n");
    return 0;    
}

static void __exit mod_cleanup(void)
{
    printk(KERN_INFO "モジュールを後始末しています。\n");
    misc_deregister(&pwn_dev);
    nf_unregister_hook(&nfho);
}


// ここで、モジュールをカーネルに「登録」している。
module_init(mod_init);
module_exit(mod_cleanup);


