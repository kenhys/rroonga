/* -*- c-file-style: "ruby" -*- */
/*
  Copyright (C) 2009  Kouhei Sutou <kou@clear-code.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "rb-grn.h"

extern grn_ctx grn_gctx;

static void
finish_groonga (VALUE data)
{
    grn_ctx *context;

    context = grn_gctx.next;
    while (RB_GRN_TRUE) {
	grn_ctx *next_context = context->next;

	grn_ctx_fin(context);
	if (next_context == &grn_gctx)
	    break;
	context = next_context;
    }
    grn_fin();
}

void
Init_groonga (void)
{
    VALUE mGrn;
    VALUE cGrnBuildVersion, cGrnBindingsVersion;

    mGrn = rb_define_module("Groonga");

    cGrnBuildVersion = rb_ary_new3(3,
				   INT2NUM(GRN_MAJOR_VERSION),
				   INT2NUM(GRN_MINOR_VERSION),
				   INT2NUM(GRN_MICRO_VERSION));
    rb_obj_freeze(cGrnBuildVersion);
    rb_define_const(mGrn, "BUILD_VERSION", cGrnBuildVersion);

    /* FIXME: API to get runtime groonga version doesn't exist */
    rb_define_const(mGrn, "VERSION", cGrnBuildVersion);

    cGrnBindingsVersion = rb_ary_new3(3,
				      INT2NUM(RB_GRN_MAJOR_VERSION),
				      INT2NUM(RB_GRN_MINOR_VERSION),
				      INT2NUM(RB_GRN_MICRO_VERSION));
    rb_obj_freeze(cGrnBindingsVersion);
    rb_define_const(mGrn, "BINDINGS_VERSION", cGrnBindingsVersion);

    rb_grn_init_utils(mGrn);
    rb_grn_init_exception(mGrn);
    rb_grn_init_encoding(mGrn);
    rb_grn_init_context(mGrn);
    rb_grn_init_object(mGrn);
    rb_grn_init_database(mGrn);
    rb_grn_init_table(mGrn);
    rb_grn_init_table_cursor(mGrn);
    rb_grn_init_type(mGrn);
    rb_grn_init_procedure(mGrn);
    rb_grn_init_column(mGrn);
    rb_grn_init_accessor(mGrn);
    rb_grn_init_record(mGrn);
    rb_grn_init_query(mGrn);
    rb_grn_init_logger(mGrn);
    rb_grn_init_snippet(mGrn);

    rb_grn_rc_check(grn_init(), Qnil);
    rb_set_end_proc(finish_groonga, Qnil);
}
