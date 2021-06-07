#! /usr/bin/python3

# standard library
import argparse
import os

# external libraries
import numpy as np
from scipy import stats
from gridData import Grid

# own library
from utilities import GridUtil


VERSION = "1.0.0"

def mask_generator(maskfile, referencefile=None, distance=5):
    if os.path.splitext(maskfile)[1] == ".pdb":
        mask = GridUtil.gen_distance_grid(referencefile, maskfile)
    else:
        mask = Grid(maskfile)

    mask.grid = mask.grid < distance
    return mask

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="combine multiple maps to a map")
    parser.add_argument("-i", metavar="IMAP", required=True, nargs="+",
                        help="input maps")
    parser.add_argument("-o", metavar="OMAP", required=True,
                        help="output map")
    parser.add_argument("--mode", default="cumulated",
                        choices=["cumulated", "probability", "zscore"],
                        help="the kind of map will be generated")
    parser.add_argument("-m", metavar="MASK",
                        help="mask dx file (storing distances to nearest protein atoms) or reference pdb file")
    parser.add_argument("--distance", metavar="d", default=5, type=int,
                        help="distance from protein atoms. it is valid only if mask is set")
    parser.add_argument("--version", action="version", version=VERSION)
    args = parser.parse_args()

    # sum up counts of each grid
    gs = [Grid(n) for n in args.i]
    ret = np.sum(gs)

    
    # generate mask: all true is mask is not provided
    mask_arr = np.full_like(ret.grid, True, dtype=bool)
    if args.m is not None:
        mask_arr = mask_generator(args.m, args.i[0], args.distance).grid

    
    # postprocess: cumulate, convert to probability, or standardize
    post_func = None
    if args.mode == "cumulated":
        post_func = lambda nparr: nparr
    elif args.mode == "probability":
        post_func = lambda nparr: nparr/np.sum(nparr)
    elif args.mode == "zscore":
        post_func = lambda nparr: stats.zscore(nparr)
    else:
        raise NotImplementedError(f"mode {args.mode} has not been implemented yet")

    values = ret.grid[np.where(mask_arr)]
    values = post_func(values)
    ret.grid = np.zeros(ret.grid.shape)
    ret.grid[np.where(mask_arr)] = values
    ret.grid[np.where(~mask_arr)] = np.min(values)

    # output processed map file
    ret.export(args.o, type="double")
