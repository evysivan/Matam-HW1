//
// Created by Yoav on 03/12/2019.
//
#include "matamazom.h"
#include "amount_set.h"
#include <stdlib.h>
#include "list.h"
#include <math.h>
#include <string.h>
#include "matamazom_print.h"
typedef  struct Product_t{
    int id;
    char* name;
    MatamazomAmountType amountType;
    MtmProductData customData;
    double income;
    MtmCopyData copyData;
    MtmGetProductPrice getProductPrice;
    MtmFreeData freeData;

} *Product;

typedef struct Order_t{
    int Order_ID;
    AmountSet Order_Products_List;

} *Order;

struct Matamazom_t{
    AmountSet Product_List;
    AmountSet Order_List;
    int Order_counter;
};
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
  /**  copy->Order_Products_List=asCopy(( ) */
    return copy;
}

static void freeOrder(ASElement order) {
    Order temp =(Order*)order;
    asDestroy(temp->Order_Products_List);
    free(temp);
}

static int compareOrder(ASElement lhs, ASElement rhs) {
    Order left=(Order*)lhs;
    Order right=(Order *)rhs;
    return (left->Order_ID-right->Order_ID);
}
/** setup#########################################555555555555555555555555555555555555555555555555555555555########## */

Matamazom matamazomCreate(){
    Matamazom matamazom= malloc(sizeof(*matamazom));
    matamazom->Product_List=asCreate(copyProduct,freeProduct,compareProducts);
    matamazom->Order_List=asCreate(copyOrder,freeOrder,compareOrder);
    matamazom->Order_counter=0;
    return matamazom;
}

void matamazomDestroy(Matamazom matamazom){
    if(matamazom==NULL)
        return;
    if (matamazom->Order_List!=NULL)
        asDestroy(matamazom->Order_List);
    if(matamazom->Product_List!=NULL)
        asDestroy(matamazom->Product_List);
    free(matamazom);
    return;
}

bool checkAmountType(MatamazomAmountType amountType, double amount){
    amount=fabs(amount);
    if (amountType==MATAMAZOM_INTEGER_AMOUNT) {
        if ((fmod(amount, 1) > 0.0001) && (fmod(amount, 1) < 0.9999))
            return false;
    }
     if (amountType==MATAMAZOM_HALF_INTEGER_AMOUNT) {
         if ((fmod(amount, 0.5) > 0.0001) && (fmod(amount, 0.5) < 0.4999))
             return false;
     }
     return true;



}

MatamazomResult mtmNewProduct(Matamazom matamazom, const unsigned int id, const char *name,
                              const double amount, const MatamazomAmountType amountType,
                              const MtmProductData customData, MtmCopyData copyData,
                              MtmFreeData freeData, MtmGetProductPrice prodPrice){
    if (matamazom==NULL||name==NULL||customData==NULL||copyData==NULL||freeData==NULL||prodPrice==NULL)
        return MATAMAZOM_NULL_ARGUMENT;
    if (!checkAmountType(amountType,amount)||amount<0)
        return MATAMAZOM_INVALID_AMOUNT;
    Product new_product=malloc(sizeof(*new_product));
    new_product->id=id;
    new_product->name=malloc(sizeof(*name));
    strcpy(new_product->name,name);
    new_product->amountType=amountType;
    new_product->income=0;
    new_product->copyData=copyData;
    new_product->freeData=freeData;
    new_product->getProductPrice=prodPrice;
    new_product->customData=new_product->copyData(customData);
    if(asRegister(matamazom->Product_List,new_product)==AS_ITEM_ALREADY_EXISTS)
        return MATAMAZOM_PRODUCT_ALREADY_EXIST;
    asChangeAmount(matamazom->Product_List,new_product,amount);
    return MATAMAZOM_SUCCESS;

}

MatamazomResult mtmChangeProductAmount(Matamazom matamazom, const unsigned int id, const double amount){
    AS_FOREACH(Product, product, matamazom->Product_List){
        if (product->id==id){
            if(!checkAmountType(product->amountType,amount))
                return MATAMAZOM_INVALID_AMOUNT;
            if(asChangeAmount(matamazom->Product_List,product,amount)==AS_INSUFFICIENT_AMOUNT)
                return  MATAMAZOM_INSUFFICIENT_AMOUNT;
            return MATAMAZOM_SUCCESS;
        }

    }
    return MATAMAZOM_PRODUCT_NOT_EXIST;
}

MatamazomResult mtmClearProduct(Matamazom matamazom, const unsigned int id){
    if(matamazom==NULL||id==NULL)
        return MATAMAZOM_NULL_ARGUMENT;
    AS_FOREACH(Product, product, matamazom->Product_List) {
        if (product->id == id) {
            AS_FOREACH(Order,order,matamazom->Order_List){
                asDelete(order->Order_Products_List,product);
            }
            asDelete(matamazom->Product_List, product);
            return MATAMAZOM_SUCCESS;
        }

    }
    return MATAMAZOM_PRODUCT_NOT_EXIST;
}


unsigned int mtmCreateNewOrder(Matamazom matamazom){
    if(matamazom==NULL)
        return 0;
    Order new_order=malloc(sizeof(*new_order));
    matamazom->Order_counter++;
    new_order->Order_ID=matamazom->Order_counter;
    new_order->Order_Products_List=asCreate(copyProduct, freeProduct, compareProducts);
    return new_order->Order_ID;
}

MatamazomResult mtmChangeProductAmountInOrder(Matamazom matamazom, const unsigned int orderId,
                                              const unsigned int productId, const double amount) {
    if (matamazom==NULL||orderId==NULL|| productId==NULL)
        return MATAMAZOM_NULL_ARGUMENT;
    AS_FOREACH(Product,productITR,matamazom->Product_List){
        if (productITR->id==productId){
            AS_FOREACH(Order,orderITR,matamazom->Order_List){
                if(orderITR->Order_ID==orderId){
                    asRegister(orderITR->Order_Products_List,productITR);
                    if(asChangeAmount(orderITR->Order_Products_List,productITR,amount)==AS_INSUFFICIENT_AMOUNT)
                        return MATAMAZOM_INSUFFICIENT_AMOUNT;
                    return MATAMAZOM_SUCCESS;
                    }
                }
            return MATAMAZOM_ORDER_NOT_EXIST;
            }

        }
    return MATAMAZOM_PRODUCT_NOT_EXIST;
}


MatamazomResult mtmShipOrder(Matamazom matamazom, const unsigned int orderId){
    double orderAmount;
    double stockAmount;
    AS_FOREACH(Order,orderITR,matamazom->Order_List){
        if (orderITR->Order_ID==orderId){
            AS_FOREACH(Product,orderProductITR,orderITR->Order_Products_List){
                asGetAmount(orderITR->Order_Products_List,orderProductITR,&orderAmount);
                asGetAmount(matamazom->Product_List,orderProductITR,&stockAmount);
                if(orderAmount>stockAmount)
                    return MATAMAZOM_INSUFFICIENT_AMOUNT;
            }
            AS_FOREACH(Product,orderProductITR,orderITR->Order_Products_List){
                asGetAmount(orderITR->Order_Products_List,orderProductITR,&orderAmount);
                asChangeAmount(matamazom->Product_List,orderProductITR,-orderAmount);
                AS_FOREACH(Product,productITR,matamazom->Product_List){
                    if (productITR->id==orderProductITR->id)
                        productITR->income+=productITR->getProductPrice(productITR->customData,orderAmount);
                }
            }
            mtmCancelOrder(matamazom,orderId);
            return MATAMAZOM_SUCCESS;
        }
    }
    return MATAMAZOM_ORDER_NOT_EXIST;
}
MatamazomResult mtmCancelOrder(Matamazom matamazom, const unsigned int orderId){
    if(matamazom==NULL||orderId==NULL)
        return MATAMAZOM_NULL_ARGUMENT;
    AS_FOREACH(Order,orderITR,matamazom->Order_List){
        if(orderITR->Order_ID==orderId){
            asDestroy(orderITR->Order_Products_List);
            asDelete(matamazom->Order_List,orderITR);
            return AS_SUCCESS;
        }
    }
    return MATAMAZOM_ORDER_NOT_EXIST;
}
/**
MatamazomResult mtmPrintInventory(Matamazom matamazom, FILE *output){
    AS_FOREACH(Product,current,matamazom->Product_List){
        mtmPrintProductDetails(current->name,current->id,);
    }
}*/