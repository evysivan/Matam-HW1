//
// Created by Yoav on 03/12/2019.
//
#include "matamazom.h"
#include "amount_set.h"
#include <stdlib.h>
#include "list.h"
typedef  struct Product_t{
    int id;
    char* name;
    double amount;
    MatamazomAmountType amountType;
    MtmProductData customData;
    double income;
    MtmCopyData copyData;
    MtmGetProductPrice getProductPrice;
    MtmFilterProduct filterProduct;
    MtmFreeData freeData;

} *Product;

typedef struct Order_t{
    int Order_ID;
    AmountSet Order_Products_List;

} *Order;

typedef struct Matamazom_t{
    AmountSet Product_List;
    List Order_List;
    int Order_counter;
    Product Best_Seller;
} *Matamazom;
/** setup#########################################555555555555555555555555555555555555555555555555555555555########## */

static ASElement copyProduct(ASElement product) {
    Product copy = malloc(sizeof(*copy));
    if (copy != NULL) {
        copy = *(Product *)product;
    }
    return copy;
}

static void freeProduct(ASElement product) {
    Product temp =(Product*)product;
    temp->freeData(temp->customData);
    free(product);
}

static int compareProducts(ASElement lhs, ASElement rhs) {
    Product left=(Product*)lhs;
    Product right=(Product*)rhs;
    return (left->id-right->id);
}
/** setup#########################################555555555555555555555555555555555555555555555555555555555########## */
static ListElement copyOrder(ListElement order) {
    Order copy = malloc(sizeof(*copy));
    if (copy != NULL) {
        copy = *(Order *)order;
    }
    copy->Order_Products_List=asCopy(( )
    return copy;
}

static void freeOrder(ASElement product) {
    Product temp =(Product*)product;
    temp->freeData(temp->customData);
    free(product);
}

static int compareOrder(ASElement lhs, ASElement rhs) {
    Product left=(Product*)lhs;
    Product right=(Product*)rhs;
    return (left->id-right->id);
}
/** setup#########################################555555555555555555555555555555555555555555555555555555555########## */

Matamazom matamazomCreate(){
    Matamazom matamazom= malloc(sizeof(*matamazom));
    matamazom->Product_List=asCreate(copyProduct,freeProduct,compareProducts);
    matamazom->Order_List=listCreate(copyOrder,freeOrder,compareOrder());
    matamazom->Best_Seller=NULL;
    return matamazom;
}

void matamazomDestroy(Matamazom matamazom){
    free(matamazom);
}

//missing stuff
MatamazomResult mtmNewProduct(Matamazom matamazom, const unsigned int id, const char *name,
                              const double amount, const MatamazomAmountType amountType,
                              const MtmProductData customData, MtmCopyData copyData,
                              MtmFreeData freeData, MtmGetProductPrice prodPrice){
    //handle allocation failure
    Product new_product=malloc(sizeof(*new_product));
    new_product->id=id;
    new_product->name=name;
    new_product->amount=amount;
    new_product->amountType=amountType;
    new_product->customData=customData;
    new_product->income=0;
    new_product->copyData=copyData;
    new_product->freeData=freeData;
    new_product->getProductPrice=prodPrice;
    asRegister(matamazom->Product_List,new_product);
}


MatamazomResult mtmChangeProductAmount(Matamazom matamazom, const unsigned int id, const double amount){
    AS_FOREACH(Product, temp, matamazom->Product_List){
        if (temp->id==id)
            asChangeAmount(matamazom->Product_List,temp,amount);
    }
}

MatamazomResult mtmClearProduct(Matamazom matamazom, const unsigned int id){
    AS_FOREACH(Product, temp, matamazom->Product_List) {
        if (temp->id == id)
            asDelete(matamazom->Product_List, temp);
    }
}
unsigned int mtmCreateNewOrder(Matamazom matamazom){
    Order new_order=malloc(sizeof(*new_order));
    matamazom->Order_counter++;
    new_order->Order_ID=matamazom->Order_counter;
    new_order->Order_Products_List=asCreate(copyProduct, freeProduct, compareProducts);
    return  new_order->Order_ID;
}

//?
MatamazomResult mtmChangeProductAmountInOrder(Matamazom matamazom, const unsigned int orderId,
                                              const unsigned int productId, const double amount) {
    LIST_FOREACH(Order, List_Iterator, matamazom->Order_List) {
        if (List_Iterator->Order_ID == orderId) {
            AS_FOREACH(Product, Iterator, List_Iterator->Order_Products_List) {
                if (Iterator->id == productId){
                    asChangeAmount(List_Iterator, Iterator, amount);
                    return MATAMAZOM_SUCCESS;
                }
            }
        }
    }
    return MATAMAZOM_PRODUCT_NOT_EXIST;
}