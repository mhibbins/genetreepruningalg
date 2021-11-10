remove(list=ls())
suppressMessages(suppressWarnings(library(tidyverse, warn.conflicts = F, quietly = T)))
library(MASS, warn.conflicts = F, quietly = T)

sim_BM <- function(t1, t2, tm, delta2, delta3, sigma2, n_traits) {
  
  
  ### Theory ###
  
  pt1_sort <- 1 - exp(-(t2 - t1))
  pt1_ILS <- (1/3)*exp(-(t2 - t1))
  pt2_sort <- 1 - exp(-(t2 - tm))
  pt2_ILS <- (1/3)*exp(-(t2 - tm))
  pt3_sort <- 1 - exp(-(t1 - tm))
  pt3_ILS <- (1/3)*exp(-(t1 - tm))
  no_intro <- 1 - (delta2 + delta3)
  
  cov_AB_1 <- (exp(t2)*(t2 - t1))/(exp(t2) - exp(t1))
  
  cov_AB <- sigma2*(no_intro*(pt1_sort*cov_AB_1 + pt1_ILS)
                    + delta2*pt2_ILS + delta3*pt3_ILS)
  
  cov_BC_1 <- (exp(t2)*(t2 - tm))/(exp(t2) - exp(tm))
  cov_BC_2 <- (exp(t1)*(t1 - tm))/(exp(t1) - exp(tm))
  
  cov_BC <- sigma2*(delta2*(pt2_sort*cov_BC_1 + pt2_ILS) + 
                      delta3*(pt3_sort*cov_BC_2 + pt3_ILS) + 
                      no_intro*pt1_ILS)
  
  cov_AC <- sigma2*(no_intro*pt1_ILS + delta2*pt2_ILS + delta3*pt3_ILS)
  
  var_all_1 <- no_intro*(pt1_sort*(t2 + 1) + exp(-(t2 - t1))*(t2 + 1 + (1/3)))
  var_all_2 <- delta2*(pt2_sort*(t2 + 1) + exp(-(t2 - tm))*(t2 + 1 + (1/3)))
  var_all_3 <- delta3*(pt3_sort*(t1 + 1) + exp(-(t1 - tm))*(t1 + 1 + (1/3)))
  
  var_all <- sigma2*(var_all_1 + var_all_2 + var_all_3)
  
  var_covar <- matrix(c(var_all, cov_AB, cov_AC,
                        cov_AB, var_all, cov_BC,
                        cov_AC, cov_BC, var_all), 
                      nrow = 3, ncol = 3)

  
  ### Trait value simulations ### 
  
  traits <- mvrnorm(n = n_traits, mu = c(0, 0, 0), Sigma = var_covar)
  return(traits)
  
}

test_traits <- sim_BM(2.7, 5.0, 0, 0, 0, 1, 100)
  
print(test_traits)
