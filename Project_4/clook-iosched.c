/*
 * elevator noop
 */
#include <linux/blkdev.h>
#include <linux/elevator.h>
#include <linux/bio.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>

static sector_t prev_sector = 0;
// #define debuging_add
// #define debuging_dispatch

struct clook_data {
	struct list_head queue;
};


static void clook_merged_requests(struct request_queue *q, struct request *rq,
				 struct request *next)
{
	list_del_init(&next->queuelist);
}

/*fills the dispatch queue with ready requests.
				I/O schedulers are free to postpone requests by
				not filling the dispatch queue unless @force
				is non-zero.  Once dispatched, I/O schedulers
				are not allowed to manipulate the requests -
				they belong to generic dispatch queue.*/
static int clook_dispatch(struct request_queue *q, int force)
{
	struct clook_data *nd = q->elevator->elevator_data;
	
	if (!list_empty(&nd ->queue)) {
		struct request *comp = NULL;
		struct list_head *curr = NULL;
		int flag = 0;
		
#ifdef debuging_dispatch
		printk("\nPrevious Sector: %llu\n",(unsigned long long)prev_sector);
		list_for_each(curr, &nd->queue) {
			comp = list_entry(curr, struct request, queuelist);
			printk("\nDispatch List Before: %llu\n",(unsigned long long)rq_end_sector(comp));
		}
		curr = NULL;
#endif
		
		list_for_each(curr, &nd->queue) {
			comp = list_entry(curr, struct request, queuelist);
			
			if(prev_sector < rq_end_sector(comp)) {
				prev_sector = rq_end_sector(comp);
				flag = 1;
				break;
			}
		}
		
		if(flag==0){
			comp = list_first_entry(&nd->queue, struct request, queuelist);
			prev_sector = rq_end_sector(comp);
		}
		
		if(rq_data_dir(comp)==0){
			printk("\n[CLOOK] dsp R %llu\n",(unsigned long long)rq_end_sector(comp));
		}
		else{
			printk("\n[CLOOK] dsp W %llu\n",(unsigned long long)rq_end_sector(comp));
		}
		
		list_del_init(&comp->queuelist);
		elv_dispatch_sort(q, comp);
		
#ifdef debuging_dispatch
// 		printk("Element cut out from the list = %llu", (unsigned long long)prev_sector);
		list_for_each(curr, &nd->queue) {
			comp = list_entry(curr, struct request, queuelist);
			printk("\nDispatch List After: %llu\n",(unsigned long long)rq_end_sector(comp));
		}
#endif
		
		return 1;
	}
	return 0;
}

/*called to add a new request into the scheduler*/
static void clook_add_request(struct request_queue *q, struct request *rq)
{
	struct clook_data *nd = q->elevator->elevator_data;
	struct list_head *curr = NULL;
	struct request *comp = NULL;
	
#ifdef debuging_add
	printk("\nRequest: %llu\n",(unsigned long long)rq_end_sector(rq));
	if(!list_empty(&nd->queue)){
		list_for_each(curr, &nd->queue) {
			comp = list_entry(curr, struct request, queuelist);
			printk("\nAdd List Before: %llu\n",(unsigned long long)rq_end_sector(comp));
		}
	}
	curr = NULL;
#endif
	
	if(rq_data_dir(rq)==0){
		printk("\n[CLOOK] add R %llu\n",(unsigned long long)rq_end_sector(rq));
	}
	else{
		printk("\n[CLOOK] add W %llu\n",(unsigned long long)rq_end_sector(rq));
	}
	
	
	list_for_each(curr, &nd->queue) {
		comp = list_entry(curr, struct request, queuelist);
		if(rq_end_sector(comp) >  rq_end_sector(rq)){
			break;
		}
	}
	list_add_tail(&rq->queuelist, curr);
	
	
#ifdef debuging_add
	list_for_each(curr, &nd->queue) {
		comp = list_entry(curr, struct request, queuelist);
		printk("\nAdd List After: %llu\n",(unsigned long long)rq_end_sector(comp));
	}
#endif
}

/*These return the request before the
				one specified in disk sort order. Used by the
				block layer to find merge possibilities.*/
static struct request *
clook_former_request(struct request_queue *q, struct request *rq)
{
	struct clook_data *nd = q->elevator->elevator_data;

	if (rq->queuelist.prev == &nd->queue)
		return NULL;
	return list_entry(rq->queuelist.prev, struct request, queuelist);
}

/*These return the request after the
				one specified in disk sort order. Used by the
				block layer to find merge possibilities.*/
static struct request *
clook_latter_request(struct request_queue *q, struct request *rq)
{
	struct clook_data *nd = q->elevator->elevator_data;

	if (rq->queuelist.next == &nd->queue)
		return NULL;
	return list_entry(rq->queuelist.next, struct request, queuelist);
}

/*Allocate elevator specific storage
				for a queue.*/
static void *clook_init_queue(struct request_queue *q)
{
	struct clook_data *nd;

	nd = kmalloc_node(sizeof(*nd), GFP_KERNEL, q->node);
	if (!nd)
		return NULL;
	INIT_LIST_HEAD(&nd->queue);
	return nd;
}

/*Free any elevator specific storage
				for a queue.*/
static void clook_exit_queue(struct elevator_queue *e)
{
	struct clook_data *nd = e->elevator_data;

	BUG_ON(!list_empty(&nd->queue));
	kfree(nd);
}

static struct elevator_type elevator_clook = {
	.ops = {
		.elevator_merge_req_fn		= clook_merged_requests,
		.elevator_dispatch_fn		= clook_dispatch,//nai
		.elevator_add_req_fn		= clook_add_request,//nai
		.elevator_former_req_fn		= clook_former_request,//isos
		.elevator_latter_req_fn		= clook_latter_request,//isos
		.elevator_init_fn		= clook_init_queue, //nai
		.elevator_exit_fn		= clook_exit_queue,//nai
	},
	.elevator_name = "clook",
	.elevator_owner = THIS_MODULE,
};

static int __init clook_init(void)
{
	elv_register(&elevator_clook);

	return 0;
}

static void __exit clook_exit(void)
{
	elv_unregister(&elevator_clook);
}

module_init(clook_init);
module_exit(clook_exit);


MODULE_AUTHOR("omada_5");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("C-LOOK IO scheduler");
